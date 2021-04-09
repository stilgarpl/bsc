//
// Created by Krzysztof Tulidowicz on 01.04.2020.
//

#include "JournalHistory.h"
bsc::JournalStatePtr bsc::JournalHistory::getLastState() {
    //@todo think: isn't history sorted? maybe this should just get last element of history
    if (!history.empty()) {
        return *std::max_element(history.begin(), history.end(),
                                 [&](auto a, auto b) -> bool {
                                   return (a->getCommitTime() < b->getCommitTime());
                                 });
    } else {
        return nullptr;
    }
}
void bsc::JournalHistory::printHistory() {
    //@todo this is debug function, remove eventually
    for (auto&& item : history) {
        std::cout << "SimpleJournal id " << item->calculateChecksum() << std::endl;
        std::cout << "SimpleJournal prev "
                  << (item->getPreviousState() ? item->getPreviousState()->calculateChecksum() : "0")
                  << std::endl;
    }
}
bool bsc::JournalHistory::merge(const bsc::JournalHistory& otherHistory) {
    decltype(history) thisCopy = history;
    decltype(history) otherCopy;
    auto thisRoot = this->findRoot();
    auto otherRoot = otherHistory.findRoot();

    if (thisRoot != nullptr && otherRoot != nullptr && *thisRoot == *otherRoot) {

        //copy other history
        for (const auto& item : otherHistory.history) {
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
                    logger.debug("MERGE FAILED!!!!!!");
                    return false;
                }
            }
        }


        history = thisCopy;
        //@todo is it right?
        return true;

    } else {
        //no common root
        return false;
    }
}
bsc::JournalStatePtr bsc::JournalHistory::getState(const bsc::CommitTimeType& commitTime, const bsc::ChecksumType& checksum) {
    auto ret = std::find_if(history.begin(), history.end(), [&](const JournalStatePtr& i) {
      return i->getCommitTime() == commitTime && i->getChecksum() == checksum;
    });

    if (ret == history.end()) {
        return nullptr;
    } else {
        return *ret;
    }
}
bsc::ChecksumType bsc::JournalHistory::calculateChecksum() {
    std::stringstream ss;
    //@todo this assumes journalHistory is sorted.
    for (const auto& item : history) {
        ss << item->getChecksum();
    }
    return ss.str();
}
void bsc::JournalHistory::insert(const bsc::JournalStatePtr& state) {
    //@todo maybe insert sort by commit time?
    history.push_back(state);
}
void bsc::JournalHistory::replay(const bsc::JournalFuncMap& funcMap) const {
    for (const auto& it : history) {
        it->replay(funcMap);
    }
}
bool bsc::JournalHistory::validate() {
return false;
}
