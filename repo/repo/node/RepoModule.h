//
// Created by stilgar on 05.11.17.
//

#ifndef BASYCO_REPOMODULE_H
#define BASYCO_REPOMODULE_H


#include <p2p/network/node/NodeModule.h>
#include <repo/repository/RepositoryManager.h>
#include <p2p/dependency/DependencyManaged.h>
#include <repo/repository/Repository.h>

class RepoModule : public NodeModule, public DependencyManaged<RepoModule> {
private:
    RepositoryManager repositoryManager;
    RepositoryPtr selectedRepository = nullptr;
public:
    RepoModule(INode &node);

    void setupActions(LogicManager &logicManager) override;

    bool assignActions(LogicManager &logicManager) override;

    bool setupSources(LogicManager &logicManager) override;

    RepositoryPtr findRepository(const Repository::RepoIdType &repoId) {
        return repositoryManager.getRepository(repoId);
    }



    ////////////////////////////////
    /// Commands section
    ////////////////////////////////

    void persistFile(const fs::path &path) {
        if (selectedRepository != nullptr) {
            selectedRepository->getJournal().append(JournalMethod::ADDED, path);
            selectedRepository->getJournal().
        }
    }

    void persistFile2(const Repository::RepoIdType &repoId, const fs::path &path) {

    }


    void selectRepository(const Repository::RepoIdType &repoId) {
        selectedRepository = findRepository(repoId);

    }

    void createRepository(const Repository::RepoIdType &repoId) {
        RepositoryPtr ptr = std::make_shared<Repository>();
        ptr->setRepositoryId(repoId);
        repositoryManager.addRepository(ptr);
    }
};


#endif //BASYCO_REPOMODULE_H
