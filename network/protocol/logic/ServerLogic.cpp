//
// Created by stilgar on 01.08.17.
//

#include "ServerLogic.h"

void ServerLogic::processPacket(std::shared_ptr<NetworkPacket> np) {

    std::cout << "Server logic" << std::endl;
    switch (np->getCommand()) {
        case Command::DISCOVERY :
            std::cout << "discovery !" << std::endl;
            break;
        case Command::ECHO :
            std::cout << "echo !" << std::endl;
            break;
        case Command::GET_NODEINFO:
            std::cout << "get nodeinfo !" << std::endl;
            break;
    }
}

