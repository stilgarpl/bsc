//
// Created by stilgar on 22.12.17.
//

#ifndef BASYCO_STORAGESYNC_H
#define BASYCO_STORAGESYNC_H


#include <p2p/network/protocol/packet/info/PacketInfo.h>
#include <repo/repository/IRepository.h>
#include <repo/journal/JournalTypes.h>

class StorageSync : public PacketGroup {

public:
    class Request : public Packet<StorageSync, StorageSync::Request> {

    private:
        IRepository::RepoIdType repositoryId;
        JournalChecksumType objectId;

        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<StorageSync, StorageSync::Request>>(this), repositoryId, objectId);
        }


        friend class cereal::access;

    public:
        const IRepository::RepoIdType &getRepositoryId() const {
            return repositoryId;
        }

        void setRepositoryId(const IRepository::RepoIdType &repositoryId) {
            Request::repositoryId = repositoryId;
        }

        const JournalChecksumType &getObjectId() const {
            return objectId;
        }

        void setObjectId(const JournalChecksumType &objectId) {
            Request::objectId = objectId;
        }

    };

};


#endif //BASYCO_STORAGESYNC_H
