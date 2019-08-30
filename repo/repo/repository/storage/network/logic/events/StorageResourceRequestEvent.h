//
// Created by stilgar on 22.12.17.
//

#ifndef BASYCO_STORAGEEVENT_H
#define BASYCO_STORAGEEVENT_H


#include <logic/IEvent.h>
#include <p2p/modules/network/protocol/logic/events/NetworkingEvent.h>
#include <repo/repository/storage/IStorage.h>

class StorageResourceRequestEvent : public NetworkingEvent<int> {
private:
    IStorage::StorageId storageId;
    IStorage::ResourceId objectId;
public:

    const IStorage::StorageId &getRepositoryId() const;

    void setRepositoryId(const IStorage::StorageId &repositoryId);

    const IStorage::ResourceId &getObjectId() const;

    void setObjectId(const IStorage::ResourceId &objectId);
};


#endif //BASYCO_STORAGEEVENT_H
