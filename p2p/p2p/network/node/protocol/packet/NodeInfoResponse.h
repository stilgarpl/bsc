//
// Created by stilgar on 08.09.17.
//

#ifndef BASYCO_NODEINFORESPONSE_H
#define BASYCO_NODEINFORESPONSE_H


#include <p2p/network/protocol/packet/BasePacket.h>
#include <p2p/network/node/NodeInfo.h>
#include "NodeInfoGroup.h"

class NodeInfoResponse : public Packet<NodeInfoGroup, NodeInfoResponse> {
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

    void process(Context::Ptr context) override;


};


CEREAL_REGISTER_TYPE(NodeInfoResponse);
//
//CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, NodeInfoResponse);


#endif //BASYCO_NODEINFORESPONSE_H
