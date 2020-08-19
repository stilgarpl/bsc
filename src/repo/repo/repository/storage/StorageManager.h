//
// Created by Krzysztof Tulidowicz on 19.08.2019.
//

#ifndef BSC_STORAGEMANAGER_H
#define BSC_STORAGEMANAGER_H


#include <map>
#include "IStorage.h"

namespace bsc {
    class StorageManager {
//@todo should manager have access to factory and create storages by itself?
        std::map<IStorage::StorageId, IStoragePtr> storages;

        IStoragePtr defaultStorage = nullptr;
    public:
        void addStorage(const IStorage::StorageId& id, const IStoragePtr& storage) {
            storages[id] = storage;
        }

        [[nodiscard]] IStoragePtr& findStorage(const IStorage::StorageId& id) {
            //@todo check if exists first?
            return storages[id];
        }


        [[nodiscard]] const IStoragePtr& getDefaultStorage() const {
            return defaultStorage;
        }

        StorageManager();

        void setDefaultStorage(const IStorage::StorageId& id, const IStoragePtr& defaultStorage);

    };
}

#endif //BSC_STORAGEMANAGER_H
