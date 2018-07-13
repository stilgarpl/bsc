//
// Created by stilgar on 08.10.17.
//

#ifndef BASYCO_NODEINFOGROUP_H
#define BASYCO_NODEINFOGROUP_H


#include <p2p/modules/nodeNetworkModule/protocol/packet/info/PacketInfo.h>



struct NodeInfoGroup : public PacketGroup {

//    typedef NodeInfoRequest Request;
//    typedef NodeInfoResponse Response;


    class Request : public Packet<NodeInfoGroup, NodeInfoGroup::Request> {
        ///@todo add optional id field - if empty, request is for this packet, if not, request is for node info for other known node
    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar & cereal::base_class<BasePacket>(this);
        }


        friend class cereal::access;

    public:
        void process(Context::Ptr context);

    };

    class Response : public Packet<NodeInfoGroup, NodeInfoGroup::Response> {
    private:
        NodeInfo nodeInfo;

    public:
        const NodeInfo &getNodeInfo() const;

        void setNodeInfo(const NodeInfo &nodeInfo);

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<BasePacket>(this), nodeInfo);
        }


        friend class cereal::access;

    public:

        void process(Context::Ptr context);


    };


};

typedef NodeInfoGroup::Request NodeInfoRequest;
typedef NodeInfoGroup::Response NodeInfoResponse;

CEREAL_REGISTER_TYPE(NodeInfoGroup::Request);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, NodeInfoGroup::Request);
CEREAL_REGISTER_TYPE(NodeInfoGroup::Response);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, NodeInfoGroup::Response);

#endif //BASYCO_NODEINFOGROUP_H
