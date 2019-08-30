//
// Created by stilgar on 22.12.17.
//

#include "StorageResourceRequestEvent.h"

const IStorage::StorageId &StorageResourceRequestEvent::getRepositoryId() const {
    return storageId;
}

void StorageResourceRequestEvent::setRepositoryId(const IStorage::StorageId &repositoryId) {
    StorageResourceRequestEvent::storageId = repositoryId;
}

const IStorage::ResourceId &StorageResourceRequestEvent::getObjectId() const {
    return objectId;
}

void StorageResourceRequestEvent::setObjectId(const IStorage::ResourceId &objectId) {
    StorageResourceRequestEvent::objectId = objectId;
}
