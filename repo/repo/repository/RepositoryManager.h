//
// Created by stilgar on 20.10.17.
//

#ifndef BASYCO_REPOSITORYMANAGER_H
#define BASYCO_REPOSITORYMANAGER_H


#include "Repository.h"

class RepositoryManager {

    std::list<RepositoryPtr> repositories;

public:
    RepositoryPtr &getRepository(Repository::RepoIdType id) {
        auto it = std::find_if(repositories.begin(), repositories.end(),
                               [&](const RepositoryPtr &rep) -> bool { return rep->getRepositoryId() == id; });
        ///@todo error checking.
        return *it;
    }

    void addRepository(RepositoryPtr ptr) {
        if (getRepository(ptr->getRepositoryId()) == nullptr) {
            repositories.push_back(ptr);
        }
    }
};


#endif //BASYCO_REPOSITORYMANAGER_H
