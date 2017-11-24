//
// Created by stilgar on 17.10.17.
//

#include "Repository.h"

const Repository::RepoIdType &Repository::getRepositoryId() const {
    return repositoryId;
}

Journal &Repository::getJournal() {
    return journal;
}

void Repository::setJournal(const Journal &journal) {
    Repository::journal = journal;
}

void Repository::setRepositoryId(const Repository::RepoIdType &repositoryId) {
    Repository::repositoryId = repositoryId;
}
