//
// Created by stilgar on 22.12.17.
//

#include "StorageSource.h"

void StorageSource::queryResource(IStorage::StorageId storageId, IStorage::ResourceId objectId,
                                  BasePacket::IdType requestId) {
    auto event = newEvent();
    event->setObjectId(objectId);
    event->setRepositoryId(storageId);
    queueEvent(event);
}
