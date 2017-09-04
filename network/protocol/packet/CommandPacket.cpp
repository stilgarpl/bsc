//
// Created by stilgar on 21.08.17.
//

#include "CommandPacket.h"
#include "../context/ConnectionContext.h"

void CommandPacket::process(Context &context) {
    logger.error("Command packet process ");

    auto info = context.get<ConnectionContext>();

//    switch (getCommand()) {
//        case Command::DISCOVERY :
//            std::cout << "discovery !" << std::endl;
//            break;
//        case Command::ECHO :
//            std::cout << "echo !" << std::endl;
//            break;
//        case Command::GET_NODEINFO:
//            std::cout << "get nodeinfo !" << std::endl;
//            break;
//    }

}

Command CommandPacket::getCommand() const {
    return command;
}

void CommandPacket::setCommand(Command command) {
    CommandPacket::command = command;
}
