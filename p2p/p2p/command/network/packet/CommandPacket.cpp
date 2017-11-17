//
// Created by stilgar on 16.11.17.
//

#include "CommandPacket.h"

void CommandPacket::Request::process(Context &context) {
    BasePacket::process(context);
}

void CommandPacket::Response::process(Context &context) {
    BasePacket::process(context);
}
