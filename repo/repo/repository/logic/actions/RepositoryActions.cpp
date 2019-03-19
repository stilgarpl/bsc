//
// Created by stilgar on 30.01.19.
//

#include <p2p/node/context/NodeContext.h>
#include <repo/node/RepoModule.h>
#include <repo/repository/network/RepoQuery.h>
#include "RepositoryActions.h"

bool RepositoryActions::checkIfUpdateRequired(const JournalPtr &currentJournal, const JournalPtr &newJournal) {
    LOGGER("REPO CHECK IF U")
    if (newJournal == nullptr || currentJournal == nullptr) {
        return false;
    }

    //@todo add more checks
    return newJournal->getChecksum() != currentJournal->getChecksum();

}

void RepositoryActions::downloadRepository(const IRepository::RepoIdType repoId) {

    auto repoMod = NodeContext::getNodeFromActiveContext().getModule<RepoModule>();

    repoMod->downloadRepository(repoId);

}
