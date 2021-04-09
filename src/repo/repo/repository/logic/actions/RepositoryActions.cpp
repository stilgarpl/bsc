//
// Created by Krzysztof Tulidowicz on 30.01.19.
//

#include <p2p/core/node/context/NodeContext.h>
#include <repo/node/RepoModule.h>
#include <repo/repository/network/RepoQuery.h>
#include "RepositoryActions.h"


namespace bsc {
    bool RepositoryActions::checkIfUpdateRequired(const JournalPtr& currentJournal, const JournalPtr& newJournal) {
        logger.debug("REPO CHECK IF U");
        if (newJournal == nullptr || currentJournal == nullptr) {
            return false;
        }

        //@todo add more checks
        return newJournal->getChecksum() != currentJournal->getChecksum();

    }

    void RepositoryActions::downloadRepository(const IRepository::RepoIdType& repoId) {

        auto repoMod = bsc::NodeContext::getNodeFromActiveContext().getModule<RepoModule>();

        repoMod->downloadRepository(repoId);

    }

    void RepositoryActions::updateJournal(const IRepository::RepoIdType& repoId, const JournalPtr& newJournal) {
        auto repoMod = bsc::NodeContext::getNodeFromActiveContext().getModule<RepoModule>();
        //@todo error handling
        //@todo shouldn't take journal, instead merge should be a function of Repository
        logger.debug("merging journal");
        repoMod->findRepository(repoId)->getJournal()->merge(newJournal);
    }

    void RepositoryActions::deployRepository(const IRepository::RepoIdType& repoId) {

        //@todo I'm wondering if those methods could be eliminated and I could just fire module commands directly.
        auto repoMod = bsc::NodeContext::getNodeFromActiveContext().getModule<RepoModule>();
        repoMod->deployRepository(repoId);

    }

    void RepositoryActions::syncRepository(const IRepository::RepoIdType& repoId) {
        auto repoMod = bsc::NodeContext::getNodeFromActiveContext().getModule<RepoModule>();
        repoMod->findRepository(repoId)->syncLocalChanges();
    }

    void RepositoryActions::saveRepository(const IRepository::RepoIdType& repoId) {
        auto repoMod = bsc::NodeContext::getNodeFromActiveContext().getModule<RepoModule>();
        repoMod->saveRepository(repoId);
    }
}