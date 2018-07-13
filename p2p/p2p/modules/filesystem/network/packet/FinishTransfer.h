//
// Created by stilgar on 30.12.17.
//

#ifndef BASYCO_FINISHTRANSFER_H
#define BASYCO_FINISHTRANSFER_H


#include <p2p/modules/nodeNetworkModule/protocol/packet/info/PacketInfo.h>
#include <p2p/modules/filesystem/identification/TransferTypes.h>


struct FinishTransfer : public PacketGroup {

public:


    class Request : public Packet<FinishTransfer, FinishTransfer::Request> {
        TransferId transferId;
    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<FinishTransfer, FinishTransfer::Request>>(this), transferId);
        }

    public:
        TransferId getTransferId() const {
            return transferId;
        }

        void setTransferId(TransferId transferId) {
            Request::transferId = transferId;
        }

        void process(Context::Ptr context) override;

    private:
        friend class cereal::access;
    };

    class Response : public Packet<FinishTransfer, FinishTransfer::Response> {
    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<FinishTransfer, FinishTransfer::Response>>(this));
        }


        friend class cereal::access;

    };
};


CEREAL_REGISTER_TYPE(FinishTransfer::Request);
CEREAL_REGISTER_TYPE(FinishTransfer::Response);

CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, FinishTransfer::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, FinishTransfer::Response);

#endif //BASYCO_FINISHTRANSFER_H
