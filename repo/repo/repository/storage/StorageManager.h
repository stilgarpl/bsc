//
// Created by stilgar on 19.08.2019.
//

#ifndef BASYCO_STORAGEMANAGER_H
#define BASYCO_STORAGEMANAGER_H


#include <map>
#include "IStorage.h"

class StorageManager {

    std::map<IStorage::StorageId, IStoragePtr> storages;

    IStoragePtr defaultStorage = nullptr;
public:
    void addStorage(const IStorage::StorageId &id, const IStoragePtr &storage) {
        storages[id] = storage;
    }

    [[nodiscard]] IStoragePtr &findStorage(const IStorage::StorageId &id) {
        return storages[id];
    }


    [[nodiscard]] const IStoragePtr &getDefaultStorage() const {
        return defaultStorage;
    }

    StorageManager();

};


#endif //BASYCO_STORAGEMANAGER_H