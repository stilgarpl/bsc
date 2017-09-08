//
// Created by stilgar on 08.09.17.
//

#ifndef BASYCO_NODEINFORESPONSE_H
#define BASYCO_NODEINFORESPONSE_H


#include "../../../protocol/packet/NetworkPacket.h"
#include "../../NodeInfo.h"

class NodeInfoResponse : public NetworkPacket {
private:
    NodeInfo nodeInfo;

public:
    const NodeInfo &getNodeInfo() const;

    void setNodeInfo(const NodeInfo &nodeInfo);

private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(cereal::base_class<NetworkPacket>(this), nodeInfo);
    }


    friend class cereal::access;

public:
    NodeInfoResponse();

    void process(Context &context) override;


};


CEREAL_REGISTER_TYPE(NodeInfoResponse);
//
//CEREAL_REGISTER_POLYMORPHIC_RELATION(NetworkPacket, NodeInfoResponse);


#endif //BASYCO_NODEINFORESPONSE_H
