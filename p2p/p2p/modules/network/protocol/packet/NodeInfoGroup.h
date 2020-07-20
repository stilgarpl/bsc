//
// Created by Krzysztof Tulidowicz on 08.10.17.
//

#ifndef BSC_NODEINFOGROUP_H
#define BSC_NODEINFOGROUP_H


#include <p2p/modules/network/protocol/packet/info/PacketInfo.h>


namespace bsc {
    struct NodeInfoGroup : public bsc::PacketGroup {

        //    typedef NodeInfoRequest Request;
        //    typedef NodeInfoResponse Response;


        class Request : public bsc::Packet<NodeInfoGroup, NodeInfoGroup::Request> {
            //@todo add optional id field - if empty, request is for this packet, if not, request is for node info for other known node
        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar & cereal::base_class<BasePacket>(this);
            }


            friend class cereal::access;


        };

        class Response : public bsc::Packet<NodeInfoGroup, NodeInfoGroup::Response> {
        private:
            NodeInfo nodeInfo;

        public:
            const NodeInfo& getNodeInfo() const;

            void setNodeInfo(const NodeInfo& nodeInfo);

        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar(cereal::base_class<BasePacket>(this), nodeInfo);
            }


            friend class cereal::access;

        public:

        };


    };
}

typedef bsc::NodeInfoGroup::Request NodeInfoRequest;
typedef bsc::NodeInfoGroup::Response NodeInfoResponse;

CEREAL_REGISTER_TYPE(bsc::NodeInfoGroup::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::NodeInfoGroup::Request)
CEREAL_REGISTER_TYPE(bsc::NodeInfoGroup::Response)
CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::NodeInfoGroup::Response)

#endif //BSC_NODEINFOGROUP_H
