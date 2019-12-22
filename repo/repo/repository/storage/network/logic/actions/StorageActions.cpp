//
// Created by stilgar on 23.12.17.
//

#include "StorageActions.h"

void StorageActions::resourceRequested(const StorageResourceRequestEvent &event) {
    auto& connectionContext = Context::getActiveContext()->get<ConnectionContext>();
    Connection& connection = connectionContext.getConnection();

    StorageQuery::Response::Ptr response = StorageQuery::Response::getNew<Status::response>(event.getRequestId());
    auto& node = NodeContext::getNodeFromActiveContext();
    auto repoMod = node.getModule<RepoModule>();

    auto repo = repoMod->findRepository(event.getRepositoryId());
    if (repo != nullptr) {
        response->setExists(repo->getStorage()->hasResource(event.getObjectId()));
    } else {
        //@todo maybe add another response? repository not found?
        response->setExists(false);
    }
    connection.send(response);
}
