//
// Created by stilgar on 08.09.17.
//

#ifndef BASYCO_NODEINFOREQUEST_H
#define BASYCO_NODEINFOREQUEST_H


#include <p2p/network/protocol/packet/BasePacket.h>
#include "NodeInfoGroup.h"


class NodeInfoRequest : public Packet<NodeInfoGroup, NodeInfoRequest> {
    ///@todo add optional id field - if empty, request is for this packet, if not, request is for node info for other known node
private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar & cereal::base_class<BasePacket>(this);
    }


    friend class cereal::access;

public:
    void process(Context::Ptr context) override;

};


CEREAL_REGISTER_TYPE(NodeInfoRequest);

CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, NodeInfoRequest);

#endif //BASYCO_NODEINFOREQUEST_H
