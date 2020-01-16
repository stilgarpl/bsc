//
// Created by stilgar on 20.10.17.
//

#ifndef BASYCO_REPOSITORYMANAGER_H
#define BASYCO_REPOSITORYMANAGER_H


#include "IRepository.h"
namespace bsc {
    class RepositoryManager {

        //@todo this probably should be a map repoId -> repoPtr
        std::list<RepositoryPtr> repositories;

    public:
        RepositoryPtr getRepository(IRepository::RepoIdType id) {
            auto it = std::find_if(repositories.begin(), repositories.end(),
                                   [&](const RepositoryPtr& rep) -> bool { return rep->getRepositoryId() == id; });
            if (it != repositories.end()) {
                return *it;
            } else {
                return nullptr;
            }

        }

        void addRepository(const RepositoryPtr& ptr) {
            if (getRepository(ptr->getRepositoryId()) == nullptr) {
                repositories.push_back(ptr);
            }
        }

        [[nodiscard]] const std::list<RepositoryPtr>& getRepositories() const {
            return repositories;
        }
    };
}

#endif //BASYCO_REPOSITORYMANAGER_H
