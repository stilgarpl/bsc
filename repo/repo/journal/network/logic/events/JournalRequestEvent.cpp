//
// Created by stilgar on 18.10.17.
//

#include "JournalRequestEvent.h"

const IRepository::RepoIdType &JournalRequestEvent::getRepoId() const {
    return repoId;
}

void JournalRequestEvent::setRepoId(const IRepository::RepoIdType &repoId) {
    JournalRequestEvent::repoId = repoId;
}

