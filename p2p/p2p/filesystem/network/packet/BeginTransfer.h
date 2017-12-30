//
// Created by stilgar on 30.12.17.
//

#ifndef BASYCO_BEGINTRANSFER_H
#define BASYCO_BEGINTRANSFER_H


#include <p2p/network/protocol/packet/info/PacketInfo.h>
#include <p2p/filesystem/identification/ResourceIdentificator.h>
#include <p2p/filesystem/identification/TransferTypes.h>

struct BeginTransfer : public PacketGroup {

public:

    class Request : public Packet<BeginTransfer, BeginTransfer::Request> {
    private:
        ResourceIdentificatorPtr resourceId;
    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<BeginTransfer, BeginTransfer::Request>>(this), resourceId);
        }

    public:
        const ResourceIdentificatorPtr &getResourceId() const {
            return resourceId;
        }

        void setResourceId(const ResourceIdentificatorPtr &resourceId) {
            Request::resourceId = resourceId;
        }

        void process(Context &context) override;

    private:

        friend class cereal::access;
    };

    class Response : public Packet<BeginTransfer, BeginTransfer::Response> {
    private:
        TransferId transferId;
    private:
        template<class Archive>
        void serialize(Archive &ar) {
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


CEREAL_REGISTER_TYPE(BeginTransfer::Request);
CEREAL_REGISTER_TYPE(BeginTransfer::Response);

CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, BeginTransfer::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, BeginTransfer::Response);

#endif //BASYCO_BEGINTRANSFER_H
