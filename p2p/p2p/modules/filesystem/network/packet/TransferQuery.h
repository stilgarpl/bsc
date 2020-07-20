//
// Created by Krzysztof Tulidowicz on 19.03.19.
//

#ifndef BSC_TRANSFERQUERY_H
#define BSC_TRANSFERQUERY_H


#include <core/utils/cereal_include.h>
#include <p2p/modules/network/protocol/packet/info/PacketInfo.h>
#include <p2p/modules/filesystem/identification/ResourceIdentificator.h>
#include <p2p/modules/filesystem/identification/TransferTypes.h>


namespace bsc {
    struct TransferQuery : public PacketGroup {

    public:


        class Request : public bsc::Packet<TransferQuery, TransferQuery::Request> {
            ResourceIdentificatorPtr resourceIdentificator;
        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar(cereal::base_class<Packet<TransferQuery, TransferQuery::Request>>(this));
                ar & resourceIdentificator;
            }

        public:
            const ResourceIdentificatorPtr& getResourceIdentificator() const {
                return resourceIdentificator;
            }

            void setResourceIdentificator(const ResourceIdentificatorPtr& resourceIdentificator) {
                Request::resourceIdentificator = resourceIdentificator;
            }

        private:

            friend class cereal::access;
        };

        class Response : public bsc::Packet<TransferQuery, TransferQuery::Response> {
            ResourceIdentificatorPtr resourceIdentificator;
            bool exists;
        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar(cereal::base_class<Packet<TransferQuery, TransferQuery::Response>>(this));
                ar & resourceIdentificator & exists;
            }

        public:

            const ResourceIdentificatorPtr& getResourceIdentificator() const {
                return resourceIdentificator;
            }

            void setResourceIdentificator(const ResourceIdentificatorPtr& resourceIdentificator) {
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
}


CEREAL_REGISTER_TYPE(bsc::TransferQuery::Request)
CEREAL_REGISTER_TYPE(bsc::TransferQuery::Response)

CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::TransferQuery::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::TransferQuery::Response)
#endif //BSC_TRANSFERQUERY_H
