//
// Created by stilgar on 22.12.17.
//

#include "StorageSource.h"

void StorageSource::queryResource(IRepository::RepoIdType repositoryId, IStorage::ResourceId objectId,
                                  BasePacket::IdType requestId) {
    auto event = newEvent();
    event->setObjectId(objectId);
    event->setRepositoryId(repositoryId);
    queueEvent(event);
}
