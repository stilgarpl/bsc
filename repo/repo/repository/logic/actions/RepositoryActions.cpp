//
// Created by stilgar on 30.01.19.
//

#include <p2p/node/context/NodeContext.h>
#include <repo/node/RepoModule.h>
#include <repo/repository/network/RepoQuery.h>
#include "RepositoryActions.h"

void RepositoryActions::getRepositoryInformation(const RepositoryEvent &repositoryEvent) {
    LOGGER("GET REPO INFO")
    auto &node = NodeContext::getNodeFromActiveContext();
    auto repo = node.getModule<RepoModule>()->findRepository(repositoryEvent.getRepoId());
    RepoQuery::Response::Ptr res = RepoQuery::Response::getNew(repositoryEvent.getRequestId());
    res->setRepoId(repositoryEvent.getRepoId());
    if (repo != nullptr) {
        res->setExists(true);
        res->setJournal(repo->getJournal());
    } else {
        res->setExists(false);
    }

    auto connectionContext = Context::getActiveContext()->get<ConnectionContext>();
    Connection &connection = connectionContext->getConnection();
    connection.send(res);


}
