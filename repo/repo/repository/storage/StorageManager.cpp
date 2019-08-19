//
// Created by stilgar on 19.08.2019.
//

#include <p2p/node/context/NodeContext.h>
#include "StorageManager.h"
#include "InternalStorage.h"

StorageManager::StorageManager() {
    //@todo take this from storage factory
    const std::string DEFAULT_STORAGE_ID = "default";
    defaultStorage = std::make_shared<InternalStorage>(DEFAULT_STORAGE_ID, "/tmp/storage/" +
                                                                           NodeContext::getNodeFromActiveContext().getNodeInfo().getNodeId() +
                                                                           "/internal");
    addStorage(defaultStorage->getStorageId(), defaultStorage);

}
