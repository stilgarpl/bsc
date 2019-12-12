//
// Created by stilgar on 22.12.17.
//

#ifndef BASYCO_STORAGEQUERY_H
#define BASYCO_STORAGEQUERY_H


#include <p2p/modules/network/protocol/packet/info/PacketInfo.h>
#include <repo/repository/IRepository.h>
#include <repo/journal/JournalTypes.h>
#include <repo/repository/storage/network/logic/sources/StorageSource.h>

//@todo to be deleted
class StorageQuery : public PacketGroup {

public:
//    enum class Query {
//        HAS,
//        GET_PATH,
//
//    };

    class Request : public Packet<StorageQuery, StorageQuery::Request> {

    private:
        IStorage::StorageId storageId;
        IStorage::ResourceId objectId;


        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<StorageQuery, StorageQuery::Request>>(this), storageId, objectId);
        }

    public:
        void process(Context::Ptr context) override;

    private:
        friend class cereal::access;

    public:
        const IStorage::StorageId &getRepositoryId() const {
            return storageId;
        }

        void setRepositoryId(const IStorage::StorageId &repositoryId) {
            Request::storageId = repositoryId;
        }

        const IStorage::ResourceId &getObjectId() const {
            return objectId;
        }

        void setObjectId(const IStorage::ResourceId &objectId) {
            Request::objectId = objectId;
        }

    };


    class Response : public Packet<StorageQuery, StorageQuery::Response> {
    private:
        //@todo C++17 path when cereal is ready
        bool exists;

        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<StorageQuery, StorageQuery::Response>>(this), exists);
        }


        friend class cereal::access;

    public:
        bool isExists() const {
            return exists;
        }

        void setExists(bool exists) {
            Response::exists = exists;
        }
    };

};

CEREAL_REGISTER_TYPE(StorageQuery::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, StorageQuery::Request)
CEREAL_REGISTER_TYPE(StorageQuery::Response)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, StorageQuery::Response)

#endif //BASYCO_STORAGEQUERY_H
