//
// Created by stilgar on 17.10.17.
//

#ifndef BSC_SIMPLEJOURNAL_H
#define BSC_SIMPLEJOURNAL_H

#include <core/utils/cereal_include.h>

#include "JournalState.h"
#include "IJournal.h"
#include "JournalTypes.h"
#include "JournalMetaDataFetcher.h"

namespace bsc {


    class SimpleJournal : public IJournal {

    private:
        ChecksumType checksum;
        JournalStatePtr currentState = nullptr;
        JournalHistory journalHistory;
        //@todo I'd like this to be const... or not embedded in journal. there should be something like a service that makes journal, journal should be pure data.
        //  so move it somewhere else. When it's moved metadatafetcher won't have to be serializable
        std::unique_ptr<JournalMetaDataFetcher> metaDataFetcher;
    private:
        template<class Archive>
        void serialize(Archive& ar) {

            ar(cereal::base_class<IJournal>(this), CEREAL_NVP(checksum), CEREAL_NVP(journalHistory),
               CEREAL_NVP(metaDataFetcher));
            //currentState is not serialized
        }


        friend class cereal::access;

        std::shared_ptr<JournalState> findLastState() {
            if (!journalHistory.empty()) {
                return *std::max_element(journalHistory.begin(), journalHistory.end(),
                                         [&](auto a, auto b) -> bool {
                                             return (a->getCommitTime() < b->getCommitTime());
                                         });
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
        void append(JournalMethod method, JournalTarget target, PathType path, bsc::FileData data) override;

        void replay(JournalFuncMap funcMap) const override;

        const std::string& calculateChecksum();

        //@todo debug method, remove eventually
        void printHistory() override {
            for (auto&& item : journalHistory) {
                std::cout << "SimpleJournal id " << item->calculateChecksum() << std::endl;
                std::cout << "SimpleJournal prev "
                          << (item->getPreviousState() ? item->getPreviousState()->calculateChecksum() : "0")
                          << std::endl;
            }
        }

        JournalStatePtr findRoot() const {
            for (auto&& item : journalHistory) {
                if (item->getPreviousState() == nullptr) {
                    return item;
                }

            }
            return nullptr;
        }


        bool merge(std::shared_ptr<SimpleJournal> other);

        bool merge(const JournalPtr& other) override;


        void replayCurrentState(JournalFuncMap funcMap) override;

        JournalStatePtr getState(const CommitTimeType& commitTime, const ChecksumType& checksumType) override;

        ~SimpleJournal() override = default;

        SimpleJournal(std::unique_ptr<JournalMetaDataFetcher> metaDataFetcher);

        SimpleJournal();

    };
}
CEREAL_REGISTER_TYPE(bsc::SimpleJournal)

CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::IJournal, bsc::SimpleJournal)

#endif //BSC_SIMPLEJOURNAL_H
