//
// Created by stilgar on 17.10.17.
//

#include "SimpleJournal.h"
#include <p2p/log/Logger.h>

JournalChecksumType SimpleJournal::getChecksum() {
    ///@todo calculate checksum?
    return checksum;
}

void SimpleJournal::commitState() {

    checkCurrentState();
    if (currentState != nullptr) {
        currentState->commit();
        currentState->calculateChecksum();
        journalHistory.push_back(currentState);
        auto prev = currentState;
        currentState = std::make_shared<JournalState>();
        currentState->setPreviousState(prev);
        ///@todo do it here?
        this->calculateChecksum();
    }
}

void SimpleJournal::replay() {

    for (auto &&it : journalHistory) {
        ///@todo I would like to remove getDataList and just pass the Func to it somehow
        for (auto &&jt : it->getDataList()) {
            LOGGER(std::to_string(jt.getMethod()) + " +++ " + jt.getPath());
            auto &func = funcMap[jt.getMethod()];
            if (func) {
                (*func)(jt);
            }
        }
    }

}

void SimpleJournal::append(JournalMethod method, PathType path) {
    prepareState();
    currentState->add(JournalStateData(method, path));
}

void SimpleJournal::prepareState() {
    if (currentState == nullptr) {
        currentState = std::make_shared<JournalState>();
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
        for (auto &&item : other->journalHistory) {
            otherCopy.push_back(std::make_shared<JournalState>(*item));
        }
        //validate other history
        otherCopy.erase(std::remove_if(otherCopy.begin(), otherCopy.end(), [](auto i) -> bool {
            auto checksum = i->getChecksum();
            return checksum != i->calculateChecksum();
        }), otherCopy.end());
        ///@todo check signature (when I add it)


        //copy all elements to thisCopy

        thisCopy.reserve(thisCopy.size() + otherCopy.size());
        thisCopy.insert(thisCopy.end(), otherCopy.begin(), otherCopy.end());
        std::sort(thisCopy.begin(), thisCopy.end(), [](auto a, auto b) {
            return a->getCommitTime() < b->getCommitTime();
        });

        thisCopy.erase(std::unique(thisCopy.begin(), thisCopy.end(), [](auto i, auto j) {
            return (i->getCommitTime() == j->getCommitTime() && i->getChecksum() == j->getChecksum());
        }), thisCopy.end());


        journalHistory = thisCopy;
        //@todo what if uncommited changes are not compatible with merged last stage? what if we are modyfing file that was deleted in last state?
        //@todo maybe we should add something to verify integrity and validity before commiting changes?
        if (currentState != nullptr) {
            currentState->setPreviousState(findLastState());
        }

    } else {
        //no common root
        return false;
    }


}

bool SimpleJournal::merge(const JournalPtr &other) {
    merge(std::static_pointer_cast<SimpleJournal>(other));
}
