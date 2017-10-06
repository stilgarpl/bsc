//
// Created by stilgar on 06.10.17.
//

#ifndef BASYCO_NETWORKINFOREQUEST_H
#define BASYCO_NETWORKINFOREQUEST_H


#include "../../../protocol/packet/BasePacket.h"

class NetworkInfoRequest : public BasePacket {
private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar & cereal::base_class<BasePacket>(this);

    }

public:
    NetworkInfoRequest();

private:

    friend class cereal::access;

public:
    void process(Context &context) override;
};

CEREAL_REGISTER_TYPE(NetworkInfoRequest);

#endif //BASYCO_NETWORKINFOREQUEST_H
