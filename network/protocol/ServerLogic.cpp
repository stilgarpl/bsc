//
// Created by stilgar on 01.08.17.
//

#include "ServerLogic.h"

void ServerLogic::processPacket(std::shared_ptr<NetworkPacket> np) {

    std::cout << "Server logic" << std::endl;
    np->print();
}

