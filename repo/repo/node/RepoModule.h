//
// Created by stilgar on 05.11.17.
//

#ifndef BASYCO_REPOMODULE_H
#define BASYCO_REPOMODULE_H


#include <p2p/network/node/NodeModule.h>
#include <repo/repository/RepositoryManager.h>
#include <p2p/dependency/DependencyManaged.h>
#include <repo/repository/Repository.h>
#include <p2p/network/node/modules/NodeNetworkModule.h>
#include <repo/journal/network/packet/JournalGroup.h>

class RepoModule : public NodeModule, public DependencyManaged<RepoModule> {
private:
    RepositoryManager repositoryManager;
    RepositoryPtr selectedRepository = nullptr;
public:
    RepoModule(INode &node);

    void setupActions(LogicManager &logicManager) override;

    bool assignActions(LogicManager &logicManager) override;

    bool setupSources(LogicManager &logicManager) override;

    RepositoryPtr findRepository(const Repository::RepoIdType &repoId);



    ////////////////////////////////
    /// Commands section
    ////////////////////////////////

    void persistFile(const fs::path &path);

    void persistFile2(const Repository::RepoIdType &repoId, const fs::path &path) {

    }


    void selectRepository(const Repository::RepoIdType &repoId);

    RepositoryPtr createRepository(const Repository::RepoIdType &repoId);

    void saveRepository(const Repository::RepoIdType &repoId);

    void loadRepository(const Repository::RepoIdType &repoId, fs::path path);

    void restoreRepository(const Repository::RepoIdType &repoId);

    void printHistory();

    void downloadRemoteRepository(const NodeIdType &remoteId, const Repository::RepoIdType &repoId) {

        auto netModule = node.getModule<NodeNetworkModule>();
        auto localRepo = findRepository(repoId);
        LOGGER("downloading repo")
        JournalGroup::Request::Ptr req = JournalGroup::Request::getNew<Status::REQUEST>();
        req->setRepoId(repoId);
        JournalGroup::Response::Ptr res = netModule->sendPacketToNode(remoteId, req);
        if (res != nullptr) {
            LOGGER("response received")
            auto remoteJournal = res->getJournal();
            if (localRepo != nullptr) {
                localRepo->getJournal()->merge(remoteJournal);
            } else {
                localRepo = createRepository(repoId);
                localRepo->setJournal(remoteJournal);

            }
            localRepo->buildFileMap();
        } else {
            LOGGER("no response")
        }


    }

    const RepositoryPtr &getSelectedRepository() const;


};


#endif //BASYCO_REPOMODULE_H
