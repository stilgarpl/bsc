//
// Created by stilgar on 17.10.17.
//

#include "SimpleJournal.h"
#include "../../../p2p/p2p/log/Logger.h"

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

void SimpleJournal::append(JournalMethod method, JournalStateData::PathType path) {
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
