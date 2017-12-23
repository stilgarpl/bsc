//
// Created by stilgar on 22.12.17.
//

#ifndef BASYCO_STORAGESYNC_H
#define BASYCO_STORAGESYNC_H


#include <p2p/network/protocol/packet/info/PacketInfo.h>
#include <repo/repository/IRepository.h>
#include <repo/journal/JournalTypes.h>
#include <p2p/network/protocol/context/LogicContext.h>
#include <repo/repository/storage/network/logic/sources/StorageSource.h>


class StorageQuery : public PacketGroup {

public:
//    enum class Query {
//        HAS,
//        GET_PATH,
//
//    };

    class Request : public Packet<StorageQuery, StorageQuery::Request> {

    private:
        IRepository::RepoIdType repositoryId;
        IStorage::ResourceId objectId;


        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<StorageQuery, StorageQuery::Request>>(this), repositoryId, objectId);
        }

    public:
        void process(Context &context) override;

    private:
        friend class cereal::access;

    public:
        const IRepository::RepoIdType &getRepositoryId() const {
            return repositoryId;
        }

        void setRepositoryId(const IRepository::RepoIdType &repositoryId) {
            Request::repositoryId = repositoryId;
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
        std::string path;

        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<StorageQuery, StorageQuery::Response>>(this), path);
        }


        friend class cereal::access;

    public:
        const std::string &getPath() const {
            return path;
        }

        void setPath(const std::string &path) {
            Response::path = path;
        }
    };

};

CEREAL_REGISTER_TYPE(StorageQuery::Request);

CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, StorageQuery::Request);

CEREAL_REGISTER_TYPE(StorageQuery::Response);

CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, StorageQuery::Response);

#endif //BASYCO_STORAGESYNC_H
