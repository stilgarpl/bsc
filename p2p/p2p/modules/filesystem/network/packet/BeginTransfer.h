//
// Created by stilgar on 30.12.17.
//

#ifndef BASYCO_BEGINTRANSFER_H
#define BASYCO_BEGINTRANSFER_H


#include <p2p/modules/network/protocol/packet/info/PacketInfo.h>
#include <p2p/modules/filesystem/identification/ResourceIdentificator.h>
#include <p2p/modules/filesystem/identification/TransferTypes.h>


namespace bsc {
    struct BeginTransfer : public PacketGroup {

    public:

        class Request : public bsc::Packet<BeginTransfer, BeginTransfer::Request> {
        private:
            ResourceIdentificatorPtr resourceId;
        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar(cereal::base_class<Packet<BeginTransfer, BeginTransfer::Request>>(this), resourceId);
            }

        public:
            const ResourceIdentificatorPtr& getResourceId() const {
                return resourceId;
            }

            void setResourceId(const ResourceIdentificatorPtr& resourceId) {
                Request::resourceId = resourceId;
            }


        private:

            friend class cereal::access;
        };

        class Response : public bsc::Packet<BeginTransfer, BeginTransfer::Response> {
        private:
            TransferId transferId;
        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar(cereal::base_class<Packet<BeginTransfer, BeginTransfer::Response>>(this), transferId);
            }

        public:
            TransferId getTransferId() const {
                return transferId;
            }

            void setTransferId(TransferId transferId) {
                Response::transferId = transferId;
            }

        private:
            friend class cereal::access;

        };
    };
}


CEREAL_REGISTER_TYPE(bsc::BeginTransfer::Request)
CEREAL_REGISTER_TYPE(bsc::BeginTransfer::Response)

CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::BeginTransfer::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::BeginTransfer::Response)
#endif //BASYCO_BEGINTRANSFER_H
