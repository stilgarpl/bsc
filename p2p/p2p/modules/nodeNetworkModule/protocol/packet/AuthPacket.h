//
// Created by stilgar on 29.08.17.
//

#ifndef BASYCO_AUTHPACKET_H
#define BASYCO_AUTHPACKET_H


#include "BasePacket.h"

class AuthPacket : public BasePacket {
private:
    //@todo zmienic to na jakis sensowny sposob autentykacji
    std::string login;
    std::string password;

private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar & cereal::base_class<BasePacket>(this);
    }


    friend class cereal::access;

};

CEREAL_REGISTER_TYPE(AuthPacket);

CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, AuthPacket);

#endif //BASYCO_AUTHPACKET_H
