//
// Created by stilgar on 17.10.17.
//

#ifndef BASYCO_JOURNAL_H
#define BASYCO_JOURNAL_H



#include "JournalState.h"
#include "IJournal.h"
#include "JournalTypes.h"
#include "JournalMetaDataFetcher.h"


class SimpleJournal : public IJournal {

private:
    ChecksumType checksum;
    JournalStatePtr currentState = nullptr;
    JournalHistory journalHistory;
    const std::unique_ptr<JournalMetaDataFetcher> metaDataFetcher;
    FuncMap funcMap;
private:
    template<class Archive>
    void serialize(Archive &ar) {

        ar(cereal::base_class<IJournal>(this), CEREAL_NVP(checksum), CEREAL_NVP(journalHistory));
        //currentState is not serialized
    }


    friend class cereal::access;
    std::shared_ptr<JournalState> findLastState() {
        if (!journalHistory.empty()) {
            return *std::max_element(journalHistory.begin(), journalHistory.end(),
                                     [&](auto a, auto b) -> bool { return (a->getCommitTime() < b->getCommitTime()); });
        } else {
            return nullptr;
        }
    }

    void checkCurrentState() {
        //@todo this is wrong. last state is commited it should be current->setPrev(last)
//        if (currentState == nullptr && journalHistory.size() > 0) {
//            currentState = findLastState();
//        }
    }

    void prepareState();

public:
    [[nodiscard]] ChecksumType getChecksum() const override;

    void commitState(CommitTimeType now) override;


    //@todo hmm hmm variadic template? - moved may have two paths for example?
    void append(JournalMethod method, JournalTarget target, PathType path, FileData data) override;

    void replay() override;

    void setFunc(const JournalMethod &method, const JournalTarget &target, Func func) override {
        funcMap[std::make_pair(method, target)] = func;
    }


    const std::string& calculateChecksum();

    //@todo debug method, remove eventually
    void printHistory() override {
        for (auto &&item : journalHistory) {
            std::cout << "SimpleJournal id " << item->calculateChecksum() << std::endl;
            std::cout << "SimpleJournal prev "
                      << (item->getPreviousState() ? item->getPreviousState()->calculateChecksum() : "0") << std::endl;
        }
    }

    JournalStatePtr findRoot() const {
        for (auto &&item : journalHistory) {
            if (item->getPreviousState() == nullptr) {
                return item;
            }

        }
        return nullptr;
    }


    bool merge(std::shared_ptr<SimpleJournal> other);

    bool merge(const JournalPtr& other) override;

    void clearFunc() override;

    void replayCurrentState() override;

    JournalStatePtr getState(const CommitTimeType& commitTime, const ChecksumType& checksumType) override;

    ~SimpleJournal() override = default;

    SimpleJournal(std::unique_ptr<JournalMetaDataFetcher> metaDataFetcher);

    SimpleJournal();

};

CEREAL_REGISTER_TYPE(SimpleJournal)

CEREAL_REGISTER_POLYMORPHIC_RELATION(IJournal, SimpleJournal)

#endif //BASYCO_JOURNAL_H
