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
        //checksum is calculated at commit and always stored. no need to recalculate it.
        return checksum;
    }

    void SimpleJournal::commitState(CommitTimeType now) {

        checkCurrentState();
        if (currentState != nullptr) {
            currentState->commit(now);
            journalHistory.push_back(currentState);
            auto prev = currentState;
            currentState = std::make_shared<JournalState>();
            currentState->setPreviousState(prev);
            //@todo do it here?
            this->calculateChecksum();
        }
    }

    void SimpleJournal::replay(JournalFuncMap funcMap) {

        for (auto&& it : journalHistory) {
            //@todo I would like to remove getDataList and just pass the Func to it somehow
            for (auto&& jt : it->getDataList()) {
                LOGGER(std::to_string(jt.getMethod()) + " +++ " + jt.getPath().string());
                funcMap.execute(jt);
            }
        }

        //@todo think if clearFunc() should not be invoked here and replay should check if any functions are setDirect. this way, if someone changes some but not all functions, replay will not do stupid things.
    }

    void SimpleJournal::replayCurrentState(JournalFuncMap funcMap) {
        if (currentState) {
            int processingPass = 0;
            while (!currentState->isProcessed()) {
                processingPass++;
                LOGGER("processing replay pass " + std::to_string(processingPass));
                //@todo I would like to remove getDataList and just pass the Func to it somehow

                for (auto&& jt : currentState->getDataList()) {
                    if (!jt.isProcessed()) {
                        LOGGER(std::to_string(jt.getMethod()) + " +++ " + jt.getPath().string());
                        funcMap.execute(jt);
                        jt.setProcessed(true);
                    }
                }
            }
            //@todo I'm not very fond of this processed flag, but it's needed so changes to journal state will be in fact processed during replay.
            currentState->clearProcessed();
        }
    }

    void SimpleJournal::append(JournalMethod method, JournalTarget target, PathType path, bsc::FileData data) {
        prepareState();
        currentState->add(JournalStateData(method, target, path, data));
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
        JournalHistory thisCopy = journalHistory;
        JournalHistory otherCopy;
        auto thisRoot = this->findRoot();
        auto otherRoot = other->findRoot();

        if (thisRoot != nullptr && otherRoot != nullptr && *thisRoot == *otherRoot) {

            //copy other history
            for (auto&& item : other->journalHistory) {
                otherCopy.push_back(std::make_shared<JournalState>(*item));
            }
            //validate other history
            otherCopy.erase(std::remove_if(otherCopy.begin(), otherCopy.end(), [](auto i) -> bool {
                                auto checksum = i->getChecksum();
                                return checksum != i->calculateChecksum();
                            }),
                            otherCopy.end());
            //@todo check signature (when I add it)


            //copy all elements to thisCopy

            thisCopy.reserve(thisCopy.size() + otherCopy.size());
            thisCopy.insert(thisCopy.end(), otherCopy.begin(), otherCopy.end());
            //sort elements
            std::sort(thisCopy.begin(), thisCopy.end(), [](auto a, auto b) {
                return a->getCommitTime() < b->getCommitTime();
            });

            //erase duplicates
            thisCopy.erase(std::unique(thisCopy.begin(), thisCopy.end(), [](auto i, auto j) {
                               return (i->getCommitTime() == j->getCommitTime() && i->getChecksum() == j->getChecksum());
                           }),
                           thisCopy.end());

            //fix previous state pointing to the wrong tree
            for (auto&& item : thisCopy) {
                if (item->getPreviousState() != nullptr) {
                    //check if previousState is in the journal
                    auto ret = std::find_if(thisCopy.begin(), thisCopy.end(), [&](const JournalStatePtr& i) {
                        return i->getCommitTime() == item->getPreviousState()->getCommitTime() &&
                               i->getChecksum() == item->getPreviousState()->getChecksum();
                    });
                    if (ret != thisCopy.end()) {
                        if (item->getPreviousState() != *ret) {
                            //different pointers
                            //fix pointer
                            item->setPreviousState(*ret);
                        } else {
                            //same pointer, ok
                        }
                    } else {
                        //@todo throw?
                        LOGGER("MERGE FAILED!!!!!!")
                        return false;
                    }
                }
            }


            journalHistory = thisCopy;
            calculateChecksum();
            //@todo what if uncommited changes are not compatible with merged last stage? what if we are modyfing file that was deleted in last state?
            //@todo maybe we should add something to verify integrity and validity before commiting changes?
            if (currentState != nullptr) {
                currentState->setPreviousState(findLastState());
            }
            //@todo is it right?
            return true;

        } else {
            //no common root
            return false;
        }
    }

    bool SimpleJournal::merge(const JournalPtr& other) {
        //@todo error handling
        return merge(std::static_pointer_cast<SimpleJournal>(other));
    }

    IJournal::JournalStatePtr SimpleJournal::getState(const CommitTimeType& commitTime, const ChecksumType& checksum) {

        auto ret = std::find_if(journalHistory.begin(), journalHistory.end(), [&](const JournalStatePtr& i) {
            return i->getCommitTime() == commitTime && i->getChecksum() == checksum;
        });

        if (ret == journalHistory.end()) {
            return nullptr;
        } else {
            return *ret;
        }
    }

    const std::string& SimpleJournal::calculateChecksum() {
        checksum = "";
        std::stringstream ss;
        std::string hash;
        {
            //@todo this assumes journalHistory is sorted.
            for (const auto& item : journalHistory) {
                ss << item->getChecksum();
            }
            //        cereal::BinaryOutputArchive oa(ss);
            //        oa << *this;
        }
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