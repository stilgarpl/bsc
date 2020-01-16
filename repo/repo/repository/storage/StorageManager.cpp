//
// Created by stilgar on 19.08.2019.
//

#include <p2p/node/context/NodeContext.h>
#include "StorageManager.h"
#include "InternalStorage.h"
namespace bsc {
    StorageManager::StorageManager() {


    }

    void StorageManager::setDefaultStorage(const IStorage::StorageId& id, const IStoragePtr& defaultStorage) {
        StorageManager::defaultStorage = defaultStorage;
        addStorage(id, defaultStorage);
    }

}