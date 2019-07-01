//
// Created by stilgar on 12.07.18.
//

#include "KeepAlivePacket.h"

void KeepAlivePacket::Request::process(Context::Ptr context) {
    BasePacket::process(context);

    auto connectionContext = context->get<ConnectionContext>();
    if (connectionContext != nullptr) {
        auto response = KeepAlivePacket::Response::getNew(this->getId());
        connectionContext->getConnection().send(response);
    } else {
        LOGGER("ERROR: No connection context!")
    }
}

void KeepAlivePacket::Response::process(Context::Ptr context) {
    BasePacket::process(context);
//    LOGGER("Keepalive response!")
}
