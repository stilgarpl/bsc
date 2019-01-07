//
// Created by stilgar on 23.12.17.
//

#ifndef BASYCO_STORAGEACTIONS_H
#define BASYCO_STORAGEACTIONS_H


#include <repo/repository/storage/network/logic/events/StorageResourceRequestEvent.h>
#include <repo/repository/storage/network/packet/StorageQuery.h>
#include <repo/node/RepoModule.h>
#include <p2p/node/context/NodeContext.h>

class StorageActions {
public:

    static void resourceRequested(const StorageResourceRequestEvent &event);
};


#endif //BASYCO_STORAGEACTIONS_H
