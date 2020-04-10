//
// Created by stilgar on 01.01.18.
//

#ifndef BSC_STORAGEMANIFEST_H
#define BSC_STORAGEMANIFEST_H

#include <core/utils/cereal_include.h>
#include <p2p/modules/network/protocol/packet/info/PacketInfo.h>
#include <repo/repository/IRepository.h>

namespace bsc {
    struct StorageManifest : public bsc::PacketGroup {

    public:


        class Request : public bsc::Packet<StorageManifest, StorageManifest::Request> {
            IStorage::StorageId repositoryId;

        private:
            template<class Archive>
            void serialize(Archive &ar) {
                ar(cereal::base_class<Packet<StorageManifest, StorageManifest::Request>>
                           (this), repositoryId);
            }

        public:
            [[nodiscard]] const IStorage::StorageId &getRepositoryId() const {
                return repositoryId;
            }

            void setRepositoryId(const IStorage::StorageId &repositoryId) {
                Request::repositoryId = repositoryId;
            }

        private:


            friend class cereal::access;
        };

        class Response : public bsc::Packet<StorageManifest, StorageManifest::Response> {
        private:
            template<class Archive>
            void serialize(Archive &ar) {
                ar(cereal::base_class<Packet<StorageManifest, StorageManifest::Response>>
                           (this));
            }


            friend class cereal::access;

        };
    };

}
CEREAL_REGISTER_TYPE(bsc::StorageManifest::Request)
CEREAL_REGISTER_TYPE(bsc::StorageManifest::Response)

CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::StorageManifest::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::StorageManifest::Response)

#endif //BSC_STORAGEMANIFEST_H
