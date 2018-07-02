//
// Created by stilgar on 22.12.17.
//

#ifndef BASYCO_STORAGESOURCE_H
#define BASYCO_STORAGESOURCE_H


#include <p2p/logic/sources/EventQueueSource.h>
#include <repo/repository/storage/network/logic/events/StorageResourceRequestEvent.h>
#include <repo/repository/IRepository.h>
#include <repo/journal/JournalTypes.h>
#include <repo/repository/storage/IStorage.h>

class StorageSource : public EventQueueSource<StorageResourceRequestEvent> {
public:

    StorageSource(SourceManager &sourceManager) : EventQueueSource(sourceManager) {}

    void
    queryResource(IRepository::RepoIdType repositoryId, IStorage::ResourceId objectId, BasePacket::IdType requestId) {
        auto event = newEvent(requestId);
        event->setObjectId(objectId);
        event->setRepositoryId(repositoryId);
        queueEvent(event);
    };
};


#endif //BASYCO_STORAGESOURCE_H