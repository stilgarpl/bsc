//
// Created by stilgar on 18.10.17.
//

#include "JournalRequestEvent.h"

const std::string &JournalRequestEvent::getRepoId() const {
    return repoId;
}

void JournalRequestEvent::setRepoId(const std::string &repoId) {
    JournalRequestEvent::repoId = repoId;
}

