//
// Created by stilgar on 12.07.18.
//

#ifndef BASYCO_KEEPALIVEPACKET_H
#define BASYCO_KEEPALIVEPACKET_H


#include "BasePacket.h"

#include <p2p/utils/cereal_include.h>
#include <p2p/modules/nodeNetworkModule/protocol/packet/info/PacketInfo.h>
#include <p2p/modules/nodeNetworkModule/protocol/context/ConnectionContext.h>

struct KeepAlivePacket : public PacketGroup {

public:


    class Request : public Packet<KeepAlivePacket, KeepAlivePacket::Request> {

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<KeepAlivePacket, KeepAlivePacket::Request>>
                       (this));
        }

    public:
        void process(Context::Ptr context) override;

    private:
        friend class cereal::access;
    };

    class Response : public Packet<KeepAlivePacket, KeepAlivePacket::Response> {
    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<KeepAlivePacket, KeepAlivePacket::Response>>
                       (this));
        }

    public:
        void process(Context::Ptr context) override;

    private:

        friend class cereal::access;

    };
};

CEREAL_REGISTER_TYPE(KeepAlivePacket::Request)
CEREAL_REGISTER_TYPE(KeepAlivePacket::Response)

CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, KeepAlivePacket::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, KeepAlivePacket::Response)

#endif //BASYCO_KEEPALIVEPACKET_H
