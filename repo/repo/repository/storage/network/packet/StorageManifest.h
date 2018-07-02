//
// Created by stilgar on 01.01.18.
//

#ifndef BASYCO_STORAGEMANIFEST_H
#define BASYCO_STORAGEMANIFEST_H

#include <p2p/utils/cereal_include.h>
#include <p2p/network/protocol/packet/info/PacketInfo.h>
#include <repo/repository/IRepository.h>

struct StorageManifest : public PacketGroup {

public:


    class Request : public Packet<StorageManifest, StorageManifest::Request> {
        IRepository::RepoIdType repositoryId;

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<StorageManifest, StorageManifest::Request>>(this), repositoryId);
        }

    public:
        const IRepository::RepoIdType &getRepositoryId() const {
            return repositoryId;
        }

        void setRepositoryId(const IRepository::RepoIdType &repositoryId) {
            Request::repositoryId = repositoryId;
        }

    private:


        friend class cereal::access;
    };

    class Response : public Packet<StorageManifest, StorageManifest::Response> {
    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<StorageManifest, StorageManifest::Response>>(this));
        }


        friend class cereal::access;

    };
};


CEREAL_REGISTER_TYPE(StorageManifest::Request);
CEREAL_REGISTER_TYPE(StorageManifest::Response);

CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, StorageManifest::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, StorageManifest::Response);

#endif //BASYCO_STORAGEMANIFEST_H