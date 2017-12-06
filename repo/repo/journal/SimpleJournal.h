//
// Created by stilgar on 17.10.17.
//

#ifndef BASYCO_JOURNAL_H
#define BASYCO_JOURNAL_H


#include <vector>
#include "JournalState.h"
#include "IJournal.h"
#include "JournalTypes.h"


class SimpleJournal : public IJournal {

private:
    JournalChecksumType checksum;
    std::shared_ptr<JournalState> currentState = nullptr;
    std::vector<std::shared_ptr<JournalState>> journalHistory;

    FuncMap funcMap;
private:
    template<class Archive>
    void serialize(Archive &ar) {

        ar(cereal::base_class<IJournal>(this), CEREAL_NVP(checksum), CEREAL_NVP(journalHistory));
        //currentState is not serialized
    }


    friend class cereal::access;

public:
    JournalChecksumType getChecksum() override;

    void commitState() override;

    void checkCurrentState() {
        if (currentState == nullptr && journalHistory.size() > 0) {
            currentState = findLastState();
        }
    }

    std::shared_ptr<JournalState> findLastState() {
        if (journalHistory.size() > 0) {
            return *std::max_element(journalHistory.begin(), journalHistory.end(),
                                     [&](auto a, auto b) -> bool { return (a->getCommitTime() < b->getCommitTime()); });
        } else {
            return nullptr;
        }
    }


    void prepareState();

    ///@todo hmm hmm variadic template?
    void append(JournalMethod method, JournalStateData::PathType path);

    void replay() override;

    void setFunc(const JournalMethod &method, Func func) {
        funcMap[method] = func;
    }


    const std::string &calculateChecksum() {
        checksum = "";
        std::stringstream ss;
        std::string hash;
        {
            cereal::BinaryOutputArchive oa(ss);
            oa << *this;
        }
        CryptoPP::SHA1 sha1;
        CryptoPP::StringSource(ss.str(), true, new CryptoPP::HashFilter(sha1, new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(hash))));
        checksum = hash;
        return checksum;
    }

    void printHistory() {
        for (auto &&item : journalHistory) {
            std::cout << "SimpleJournal id " << item->calculateChecksum() << std::endl;
            std::cout << "SimpleJournal prev "
                      << (item->getPreviousState() ? item->getPreviousState()->calculateChecksum() : "0") << std::endl;
        }
    }

};

CEREAL_REGISTER_TYPE(SimpleJournal)

CEREAL_REGISTER_POLYMORPHIC_RELATION(IJournal, SimpleJournal);

#endif //BASYCO_JOURNAL_H
