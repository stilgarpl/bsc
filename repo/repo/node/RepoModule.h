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

class RepoModule : public NodeModuleDependent<RepoModule> {
private:
    RepositoryManager repositoryManager;
    RepositoryPtr selectedRepository = nullptr;
public:
    RepoModule(INode &node);

    void setupActions(ILogicModule::SetupActionHelper &actionHelper) override;

    bool assignActions(ILogicModule::AssignActionHelper &actionHelper) override;

    bool setupSources(ILogicModule::SetupSourceHelper &sourceHelper) override;

    RepositoryPtr findRepository(const Repository::RepoIdType &repoId);



    ////////////////////////////////
    /// Commands section
    ////////////////////////////////

    void persistFile(const fs::path &path);

    void updateFile(const fs::path &path);

    void updateAllFiles();

    void persistFile2(const Repository::RepoIdType &repoId, const fs::path &path) {

    }


    void selectRepository(const Repository::RepoIdType &repoId);

    RepositoryPtr createRepository(const Repository::RepoIdType &repoId);

    void saveRepository(const Repository::RepoIdType &repoId);

    void loadRepository(const Repository::RepoIdType &repoId, fs::path path);

    void restoreRepository(const Repository::RepoIdType &repoId);

    void printHistory();

    void downloadRemoteRepository(const NodeIdType &remoteId, const Repository::RepoIdType &repoId);

//
//    void requestStoragePath(const NodeIdType &remoteId, const Repository::RepoIdType &repoId,
//                            const IStorage::ResourceId &resourceId) {
//        auto netModule = node.getModule<NodeNetworkModule>();
//        auto localRepo = findRepository(repoId);
//
//        StorageQuery::Request::Ptr req = StorageQuery::Request::getNew();
//        req->setRepositoryId(repoId);
//        req->setObjectId(resourceId);
//
//        StorageQuery::Response::Ptr res = netModule->sendPacketToNode(remoteId, req);
//        if (res != nullptr) {
//            LOGGER("resouce path: " + res->getPath());
//        } else {
//            LOGGER("invalid response");
//        }
//    }
    const RepositoryPtr &getSelectedRepository() const;


};


#endif //BASYCO_REPOMODULE_H
