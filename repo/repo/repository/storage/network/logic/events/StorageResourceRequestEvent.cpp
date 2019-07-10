//
// Created by stilgar on 22.12.17.
//

#include "StorageResourceRequestEvent.h"

const IRepository::RepoIdType &StorageResourceRequestEvent::getRepositoryId() const {
    return repositoryId;
}

void StorageResourceRequestEvent::setRepositoryId(const IRepository::RepoIdType &repositoryId) {
    StorageResourceRequestEvent::repositoryId = repositoryId;
}

const IStorage::ResourceId &StorageResourceRequestEvent::getObjectId() const {
    return objectId;
}

void StorageResourceRequestEvent::setObjectId(const IStorage::ResourceId &objectId) {
    StorageResourceRequestEvent::objectId = objectId;
}
