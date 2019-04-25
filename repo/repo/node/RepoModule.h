//
// Created by stilgar on 05.11.17.
//

#ifndef BASYCO_REPOMODULE_H
#define BASYCO_REPOMODULE_H


#include <p2p/node/NodeModule.h>
#include <repo/repository/RepositoryManager.h>
#include <p2p/dependency/DependencyManaged.h>
#include <repo/repository/Repository.h>
#include <p2p/modules/nodeNetworkModule/NodeNetworkModule.h>
#include <repo/journal/network/packet/JournalGroup.h>
#include <repo/repository/storage/network/packet/StorageQuery.h>

class RepoModuleConfiguration : public IConfig {
private:
    fs::path repositoryDataPath = fs::path("repository");

public:
    const std::filesystem::path &getRepositoryDataPath() const;

    void setRepositoryDataPath(const std::filesystem::path &repositoryDataPath);

private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(repositoryDataPath);
    }


    friend class cereal::access;
};

class RepoModule : public NodeModuleConfigDependent<RepoModule,RepoModuleConfiguration> {
private:
    RepositoryManager repositoryManager;
    RepositoryPtr selectedRepository = nullptr;
public:
    explicit RepoModule(INode &node);

    void setupActions(ILogicModule::SetupActionHelper &actionHelper) override;

    bool assignActions(ILogicModule::AssignActionHelper &actionHelper) override;

    bool setupSources(ILogicModule::SetupSourceHelper &sourceHelper) override;

    RepositoryPtr findRepository(const Repository::RepoIdType &repoId);



    ////////////////////////////////
    /// Commands section
    ////////////////////////////////

    void persistFile(const fs::path &path);

    void ignoreFile(const fs::path &path);

    void updateFile(const fs::path &path);

    void updateAllFiles();

    void persistFile(const Repository::RepoIdType &repoId, const fs::path &path);


    void selectRepository(const Repository::RepoIdType &repoId);

    RepositoryPtr createRepository(const Repository::RepoIdType &repoId);

    void saveRepository(const Repository::RepoIdType &repoId);

    void loadRepository(const Repository::RepoIdType &repoId);

    void restoreRepository(const Repository::RepoIdType &repoId);

    void printHistory();

    void downloadRemoteRepository(const NodeIdType &remoteId, const Repository::RepoIdType &repoId);

    void downloadRepository(const Repository::RepoIdType &repoId);

    void deployRepository(const Repository::RepoIdType &repoId);
    void findNewestRepositoryInTheNetwork(const Repository::RepoIdType &repoId) {
        //broadcast to every node get repo info packet
        //for every response:
        //  check if it has something this node hasn't
        //  if it does, download repo -> update repo
    }


    const RepositoryPtr &getSelectedRepository() const;


    void prepareSubmodules() override;

};


#endif //BASYCO_REPOMODULE_H
