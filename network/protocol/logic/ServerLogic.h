//
// Created by stilgar on 01.08.17.
//

#ifndef BASYCO_SERVERLOGIC_H
#define BASYCO_SERVERLOGIC_H


#include "IServerLogic.h"

class ServerLogic : public IServerLogic {
public:


    void processPacket(std::shared_ptr<NetworkPacket> np) override;


};


#endif //BASYCO_SERVERLOGIC_H
