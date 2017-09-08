//
// Created by stilgar on 08.09.17.
//

#ifndef BASYCO_NODEINFOREQUEST_H
#define BASYCO_NODEINFOREQUEST_H


#include "../../../protocol/packet/NetworkPacket.h"

class NodeInfoRequest : public NetworkPacket {
    ///@todo add optional id field - if empty, request is for this packet, if not, request is for node info for other known node
private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar & cereal::base_class<NetworkPacket>(this);
    }


    friend class cereal::access;

public:
    void process(Context &context) override;

    NodeInfoRequest();
};


CEREAL_REGISTER_TYPE(NodeInfoRequest);

CEREAL_REGISTER_POLYMORPHIC_RELATION(NetworkPacket, NodeInfoRequest);

#endif //BASYCO_NODEINFOREQUEST_H
