//
// Created by stilgar on 17.10.17.
//

#include "Journal.h"
#include "../../../p2p/p2p/log/Logger.h"

JournalChecksumType Journal::getChecksum() {
    ///@todo calculate checksum?
    return checksum;
}

void Journal::commitState() {

    if (currentState != nullptr) {
        currentState->commit();
        LOGGER(currentState->calculateChecksum());
        journalHistory.push_back(*currentState);
        currentState = nullptr;
    }
}

void Journal::replay() {

    for (auto &&it : journalHistory) {
        ///@todo I would like to remove getDataList and just pass the Func to it somehow
        for (auto &&jt : it.getDataList()) {
            LOGGER(std::to_string(jt.getMethod()) + " +++ " + jt.getPath());
            auto &func = funcMap[jt.getMethod()];
            if (func) {
                (*func)(jt);
            }
        }
    }

}

void Journal::append(JournalMethod method, JournalStateData::PathType path) {
    prepareState();
    currentState->add(JournalStateData(method, path));
}

void Journal::prepareState() {
    if (currentState == nullptr) {
        currentState = std::make_shared<JournalState>();
    }
}
