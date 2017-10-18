//
// Created by stilgar on 17.10.17.
//

#ifndef BASYCO_JOURNAL_H
#define BASYCO_JOURNAL_H


#include <vector>
#include "JournalState.h"
#include "IJournal.h"
#include "JournalTypes.h"

class Journal : public IJournal {

private:
    JournalChecksumType checksum;
    std::shared_ptr<JournalState> currentState = nullptr;
    std::vector<std::shared_ptr<JournalState>> journalHistory;

    FuncMap funcMap;
private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(checksum, journalHistory);
        //currentState is not serialized
    }


    friend class cereal::access;

public:
    JournalChecksumType getChecksum() override;

    void commitState() override;

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

};


#endif //BASYCO_JOURNAL_H
