//
// Created by stilgar on 01.08.17.
//

#include "../context/Context.h"
#include "NetworkPacket.h"

void NetworkPacket::process(Context &context) {
    std::cout << "Network packet process ";
    switch (getCommand()) {
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
