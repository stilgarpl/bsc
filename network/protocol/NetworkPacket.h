//
// Created by stilgar on 01.08.17.
//

#ifndef BASYCO_NETWORKPACKET_H
#define BASYCO_NETWORKPACKET_H


#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/polymorphic.hpp>


class NetworkPacket {

public:

private:
    template<class Archive>
    void serialize(Archive &ar) {
    }


    friend class cereal::access;
};


CEREAL_REGISTER_TYPE(NetworkPacket);

#endif //BASYCO_NETWORKPACKET_H
