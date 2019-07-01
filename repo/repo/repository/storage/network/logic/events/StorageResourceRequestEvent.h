//
// Created by stilgar on 22.12.17.
//

#ifndef BASYCO_STORAGEEVENT_H
#define BASYCO_STORAGEEVENT_H


#include <logic/IEvent.h>
#include <p2p/modules/nodeNetworkModule/protocol/logic/events/NetworkingEvent.h>
#include <repo/repository/storage/IStorage.h>

class StorageResourceRequestEvent : public NetworkingEvent<int> {
private:
    IRepository::RepoIdType repositoryId;
    IStorage::ResourceId objectId;
public:
    StorageResourceRequestEvent(BasePacket::IdType requestId);

    const IRepository::RepoIdType &getRepositoryId() const;

    void setRepositoryId(const IRepository::RepoIdType &repositoryId);

    const IStorage::ResourceId &getObjectId() const;

    void setObjectId(const IStorage::ResourceId &objectId);
};


#endif //BASYCO_STORAGEEVENT_H
