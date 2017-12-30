//
// Created by stilgar on 30.12.17.
//

#ifndef BASYCO_FINISHTRANSFER_H
#define BASYCO_FINISHTRANSFER_H


#include <p2p/network/protocol/packet/info/PacketInfo.h>

struct FinishTransfer : public PacketGroup {

public:


    class Request : public Packet<FinishTransfer, FinishTransfer::Request> {

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<FinishTransfer, FinishTransfer::Request>>(this));
        }


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
