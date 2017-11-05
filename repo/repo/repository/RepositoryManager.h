//
// Created by stilgar on 20.10.17.
//

#ifndef BASYCO_REPOSITORYMANAGER_H
#define BASYCO_REPOSITORYMANAGER_H


#include "Repository.h"

class RepositoryManager {

    std::list<Repository> repositories;

    Repository &getRepository(Repository::RepoIdType id) {
        auto it = std::find_if(repositories.begin(), repositories.end(),
                               [&](const Repository &rep) -> bool { return rep.getRepositoryId() == id; });
        ///@todo error checking.
        return *it;
    }

};


#endif //BASYCO_REPOSITORYMANAGER_H
