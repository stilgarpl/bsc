//
// Created by stilgar on 12.07.18.
//

#ifndef BASYCO_KEEPALIVEPACKET_H
#define BASYCO_KEEPALIVEPACKET_H


#include <core/utils/cereal_include.h>
#include <p2p/modules/network/protocol/packet/info/PacketInfo.h>
#include <p2p/modules/network/protocol/context/ConnectionContext.h>

namespace bsc {
    struct KeepAlivePacket : public bsc::PacketGroup {

    public:


        class Request : public bsc::Packet<KeepAlivePacket, KeepAlivePacket::Request> {

        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar(cereal::base_class<Packet<KeepAlivePacket, KeepAlivePacket::Request>>
                           (this));
            }

        public:


        private:
            friend class cereal::access;
        };

        class Response : public bsc::Packet<KeepAlivePacket, KeepAlivePacket::Response> {
        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar(cereal::base_class<Packet<KeepAlivePacket, KeepAlivePacket::Response>>
                           (this));
            }

        private:

            friend class cereal::access;

        };
    };
}

CEREAL_REGISTER_TYPE(bsc::KeepAlivePacket::Request)
CEREAL_REGISTER_TYPE(bsc::KeepAlivePacket::Response)

CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::KeepAlivePacket::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::KeepAlivePacket::Response)

#endif //BASYCO_KEEPALIVEPACKET_H
