//
// Created by stilgar on 06.10.17.
//

#ifndef BASYCO_NETWORKINFOREQUEST_H
#define BASYCO_NETWORKINFOREQUEST_H


#include "NetworkInfoGroup.h"

class NetworkInfoRequest : public Packet<NetworkInfoGroup, NetworkInfoRequest> {
private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar & cereal::base_class<BasePacket>(this);

    }

private:

    friend class cereal::access;

};

CEREAL_REGISTER_TYPE(NetworkInfoRequest)

#endif //BASYCO_NETWORKINFOREQUEST_H
