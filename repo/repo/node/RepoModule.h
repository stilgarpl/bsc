//
// Created by stilgar on 05.11.17.
//

#ifndef BASYCO_REPOMODULE_H
#define BASYCO_REPOMODULE_H


#include <p2p/network/node/NodeModule.h>
#include <repo/repository/RepositoryManager.h>
#include <p2p/dependency/DependencyManaged.h>
#include <repo/repository/Repository.h>

class RepoModule : public NodeModule, DependencyManaged<RepoModule> {
private:
    RepositoryManager repositoryManager;
public:
    RepoModule(INode &node);

    void setupActions(LogicManager &logicManager) override;

    bool assignActions(LogicManager &logicManager) override;

    bool setupSources(LogicManager &logicManager) override;

    Repository &findRepository(std::string repoId) {
        return repositoryManager.getRepository(repoId);
    }
};


#endif //BASYCO_REPOMODULE_H
