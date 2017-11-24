//
// Created by stilgar on 17.10.17.
//

#ifndef BASYCO_REPOSITORY_H
#define BASYCO_REPOSITORY_H


#include <repo/journal/Journal.h>

class Repository {
public:
    typedef std::string RepoIdType;
private:
    Journal journal;
    RepoIdType repositoryId;

public:
    const RepoIdType &getRepositoryId() const;

    Journal &getJournal();

    void setJournal(const Journal &journal);

    void setRepositoryId(const RepoIdType &repositoryId);

public:

};

typedef std::shared_ptr<Repository> RepositoryPtr;

#endif //BASYCO_REPOSITORY_H
