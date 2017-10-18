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
        journalHistory.push_back(currentState);
    }
}

void Journal::replay() {

    for (auto &&it : journalHistory) {
        for (auto &&jt : it->getDataList()) {
            LOGGER(std::to_string(jt.getMethod()) + " +++ " + jt.getPath());
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
