//
// Created by stilgar on 20.10.17.
//

#ifndef BASYCO_REPOSITORYMANAGER_H
#define BASYCO_REPOSITORYMANAGER_H


#include "Repository.h"

class RepositoryManager {

    std::list<RepositoryPtr> repositories;

public:
    RepositoryPtr getRepository(IRepository::RepoIdType id) {
        auto it = std::find_if(repositories.begin(), repositories.end(),
                               [&](const RepositoryPtr &rep) -> bool { return rep->getRepositoryId() == id; });
        if (it != repositories.end()) {
            return *it;
        } else {
            return nullptr;
        }

    }

    void addRepository(RepositoryPtr ptr) {
        if (getRepository(ptr->getRepositoryId()) == nullptr) {
            repositories.push_back(ptr);
        }
    }

    const std::list<RepositoryPtr> &getRepositories() const {
        return repositories;
    }
};


#endif //BASYCO_REPOSITORYMANAGER_H
