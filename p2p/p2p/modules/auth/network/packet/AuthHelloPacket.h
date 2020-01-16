//
// Created by stilgar on 19.07.18.
//

#ifndef BASYCO_AUTHHELLOPACKET_H
#define BASYCO_AUTHHELLOPACKET_H


#include <core/utils/cereal_include.h>
#include <p2p/modules/network/protocol/packet/info/PacketInfo.h>
#include <p2p/node/NodeInfo.h>


namespace bsc {
    enum class AuthResult {
        UNKNOWN_NODE,
        AUTHENTICATION_FAILED,
        AUTHENTICATED,
    };
    struct AuthHelloPacket : public PacketGroup {

    public:


        class Request : public bsc::Packet<AuthHelloPacket, AuthHelloPacket::Request> {
            NodeIdType nodeId;
            std::string authData;

        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar(cereal::base_class<Packet<AuthHelloPacket, AuthHelloPacket::Request>>(this));
                ar(nodeId, authData);
            }

        public:
            //        void process(Context::Ptr context) override;

        private:
            friend class cereal::access;
        };

        class Response : public bsc::Packet<AuthHelloPacket, AuthHelloPacket::Response> {
            AuthResult result;
        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar(cereal::base_class<Packet<AuthHelloPacket, AuthHelloPacket::Response>>(this));
                ar(result);
            }


            friend class cereal::access;

        };
    };
}


CEREAL_REGISTER_TYPE(bsc::AuthHelloPacket::Request)
CEREAL_REGISTER_TYPE(bsc::AuthHelloPacket::Response)

CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::AuthHelloPacket::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::AuthHelloPacket::Response)
#endif //BASYCO_AUTHHELLOPACKET_H
