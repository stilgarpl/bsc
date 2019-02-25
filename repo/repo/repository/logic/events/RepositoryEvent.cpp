//
// Created by stilgar on 06.11.18.
//

#include "RepositoryEvent.h"

const IRepository::RepoIdType &RepositoryEvent::getRepoId() const {
    return repoId;
}

void RepositoryEvent::setRepoId(const IRepository::RepoIdType &repoId) {
    RepositoryEvent::repoId = repoId;
}
