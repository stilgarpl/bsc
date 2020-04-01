//
// Created by stilgar on 17.10.17.
//
#include "SimpleJournal.h"
#include "SimpleJournalMetaDataFetcher.h"
#include <core/utils/crypto.h>
#include <utility>

#include <core/log/Logger.h>

namespace bsc {
    ChecksumType SimpleJournal::getChecksum() const {
        //checksum is calculated at commit and merge and always stored. no need to recalculate it.
        return checksum;
    }

    void SimpleJournal::commitState(CommitTimeType now) {

        checkCurrentState();
        if (currentState != nullptr) {
            currentState->commit(now);
            journalHistory.insert(currentState);
            auto prev = currentState;
            currentState = std::make_shared<JournalState>();
            currentState->setPreviousState(prev);
            //@todo do it here?
            this->calculateChecksum();
        }
    }

    void SimpleJournal::replay(const JournalFuncMap& funcMap) const {
        journalHistory.replay(funcMap);

    }

    void SimpleJournal::replayCurrentState(const JournalFuncMap&  funcMap) {
        if (currentState) {
         currentState->replay(funcMap);
        }
    }

    void SimpleJournal::append(JournalMethod method, JournalTarget target, PathType path, bsc::FileData data) {
        prepareState();
        if(data.isIsDirectory()) {
            currentState->add(JournalStateData<JournalTarget::directory>(method, path, data));
        } else {
            currentState->add(JournalStateData<JournalTarget::file>(method, path, data));
        }
    }

    void SimpleJournal::prepareState() {
        if (currentState == nullptr) {
            currentState = std::make_shared<JournalState>(metaDataFetcher->makeMetaData());
            //        if (journalHistory.size() > 0) {
            //            currentState->setPreviousState(journalHistory.back());
            //        }
            currentState->setPreviousState(findLastState());
        }
    }

    bool SimpleJournal::merge(const std::shared_ptr<SimpleJournal> other) {
        bool result =  journalHistory.merge(other->journalHistory);

        calculateChecksum();
        if (result ) {
            //@todo what if uncommited changes are not compatible with merged last stage? what if we are modyfing file that was deleted in last state?
            //@todo maybe we should add something to verify integrity and validity before commiting changes?
            if (currentState != nullptr) {
                currentState->setPreviousState(findLastState());
            }
        }
        return result;
    }

    bool SimpleJournal::merge(const JournalPtr& other) {
        //@todo error handling if other is not SimpleJournal -- or maybe just add protected getHistory to IJournal, so there may be just one overload
        return merge(std::static_pointer_cast<SimpleJournal>(other));
    }

    JournalStatePtr SimpleJournal::getState(const CommitTimeType& commitTime, const ChecksumType& checksum) {

        return journalHistory.getState(commitTime,checksum);

    }

    const std::string& SimpleJournal::calculateChecksum() {
        checksum = "";
        std::stringstream ss;
        std::string hash;
        ss << journalHistory.calculateChecksum();


        std::string source = ss.str();
        hash = bsc::calculateSha1OfString(source);
        //    LOGGER("string is " + source)
        //    LOGGER("hash is " + hash);
        checksum = hash;
        return checksum;
    }

    SimpleJournal::SimpleJournal(std::unique_ptr<JournalMetaDataFetcher> metaDataFetcher) : metaDataFetcher(
                                                                                                    std::move(metaDataFetcher)) {}

    SimpleJournal::SimpleJournal() : metaDataFetcher(std::make_unique<SimpleJournalMetaDataFetcher>()) {}
}// namespace bsc