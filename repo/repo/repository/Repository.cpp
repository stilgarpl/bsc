//
// Created by stilgar on 17.10.17.
//

#include "Repository.h"

const Repository::RepoIdType &Repository::getRepositoryId() const {
    return repositoryId;
}


JournalPtr &Repository::getJournal() {
    return journal;
}

void Repository::setJournal(const JournalPtr &journal) {
    Repository::journal = journal;
}

Repository::Repository(const Repository::RepoIdType &repositoryId) : repositoryId(repositoryId),
                                                                     storage(std::make_shared<InternalStorage>(
                                                                             repositoryId)) {}
