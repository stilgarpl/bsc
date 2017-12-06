//
// Created by stilgar on 17.10.17.
//

#include "Repository.h"

const Repository::RepoIdType &Repository::getRepositoryId() const {
    return repositoryId;
}


void Repository::setRepositoryId(const Repository::RepoIdType &repositoryId) {
    Repository::repositoryId = repositoryId;
}

JournalPtr &Repository::getJournal() {
    return journal;
}

void Repository::setJournal(const JournalPtr &journal) {
    Repository::journal = journal;
}
