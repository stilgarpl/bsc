//
// Created by stilgar on 01.08.17.
//

#ifndef BASYCO_ISERVERLOGIC_H
#define BASYCO_ISERVERLOGIC_H


#include "../packet/NetworkPacket.h"

class IServerLogic {
public:


    virtual void processPacket(std::shared_ptr<NetworkPacket> np)= 0;
};


#endif //BASYCO_ISERVERLOGIC_H
