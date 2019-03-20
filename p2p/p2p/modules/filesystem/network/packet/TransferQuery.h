//
// Created by stilgar on 19.03.19.
//

#ifndef BASYCO_TRANSFERQUERY_H
#define BASYCO_TRANSFERQUERY_H


#include <p2p/utils/cereal_include.h>
#include <p2p/modules/nodeNetworkModule/protocol/packet/info/PacketInfo.h>
#include <p2p/modules/filesystem/identification/ResourceIdentificator.h>
#include <p2p/modules/filesystem/identification/TransferTypes.h>

struct TransferQuery : public PacketGroup {

public:


    class Request : public Packet<TransferQuery, TransferQuery::Request> {
        ResourceIdentificatorPtr resourceIdentificator;
    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<TransferQuery, TransferQuery::Request>>(this));
            ar & resourceIdentificator;
        }

    public:
        const ResourceIdentificatorPtr &getResourceIdentificator() const {
            return resourceIdentificator;
        }

        void setResourceIdentificator(const ResourceIdentificatorPtr &resourceIdentificator) {
            Request::resourceIdentificator = resourceIdentificator;
        }

    private:

        friend class cereal::access;
    };

    class Response : public Packet<TransferQuery, TransferQuery::Response> {
        ResourceIdentificatorPtr resourceIdentificator;
        bool exists;
    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<TransferQuery, TransferQuery::Response>>(this));
            ar & resourceIdentificator & exists;
        }

    public:

        const ResourceIdentificatorPtr &getResourceIdentificator() const {
            return resourceIdentificator;
        }

        void setResourceIdentificator(const ResourceIdentificatorPtr &resourceIdentificator) {
            Response::resourceIdentificator = resourceIdentificator;
        }

        bool isExists() const {
            return exists;
        }

        void setExists(bool exists) {
            Response::exists = exists;
        }

    private:

        friend class cereal::access;

    };
};


CEREAL_REGISTER_TYPE(TransferQuery::Request)
CEREAL_REGISTER_TYPE(TransferQuery::Response)

CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, TransferQuery::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, TransferQuery::Response)

#endif //BASYCO_TRANSFERQUERY_H
