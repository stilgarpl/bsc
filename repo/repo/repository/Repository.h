//
// Created by stilgar on 17.10.17.
//

#ifndef BASYCO_REPOSITORY_H
#define BASYCO_REPOSITORY_H


#include <repo/journal/SimpleJournal.h>

class Repository {
public:
    typedef std::string RepoIdType;
private:
    JournalPtr journal = std::make_shared<SimpleJournal>();
    RepoIdType repositoryId;

public:
    const RepoIdType &getRepositoryId() const;

    const JournalPtr &getJournal() const;

    void setJournal(const JournalPtr &journal);

    void setRepositoryId(const RepoIdType &repositoryId);

public:

};

typedef std::shared_ptr<Repository> RepositoryPtr;

#endif //BASYCO_REPOSITORY_H
