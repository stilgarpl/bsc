//
// Created by stilgar on 16.11.17.
//

#include <p2p/command/network/logic/sources/CommandSource.h>
#include <p2p/network/protocol/context/LogicContext.h>
#include "CommandPacket.h"

void CommandPacket::Request::process(Context &context) {
    BasePacket::process(context);
    auto lc = context.get<LogicContext>();
    if (lc != nullptr) {
        auto commnandSource = lc->getLogicManager().getSource<CommandSource>();
        commnandSource->commandReceived(this->modules, this->commandName, this->data);
    }
}

void CommandPacket::Response::process(Context &context) {
    BasePacket::process(context);
}
