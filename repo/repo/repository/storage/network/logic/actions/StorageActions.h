//
// Created by stilgar on 23.12.17.
//

#ifndef BASYCO_STORAGEACTIONS_H
#define BASYCO_STORAGEACTIONS_H


#include <repo/repository/storage/network/logic/events/StorageResourceRequestEvent.h>
#include <repo/repository/storage/network/packet/StorageQuery.h>
#include <repo/node/RepoModule.h>

class StorageActions {
public:

    static void resourceRequested(const StorageResourceRequestEvent &event) {
        Connection *connection = event.origin();

        StorageQuery::Response::Ptr response = StorageQuery::Response::getNew<Status::RESPONSE>(event.getRequestId());
        auto &node = NodeContext::getNodeFromActiveContext();
        auto repoMod = node.getModule<RepoModule>();

        auto repo = repoMod->findRepository(event.getRepositoryId());
        if (repo != nullptr) {
            response->setExists(repo->getStorage()->hasResource(event.getObjectId()));
        } else {
            ///@todo maybe add another response? repository not found?
            response->setExists(false);
        }
        connection->send(response);
    }
};


#endif //BASYCO_STORAGEACTIONS_H
