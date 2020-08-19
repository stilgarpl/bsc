//
// Created by Krzysztof Tulidowicz on 29.08.17.
//

#include "ProtocolActions.h"


void bsc::ProtocolActions::onNewConnection(const ConnectionEvent& connectionEvent) {

    std::clog << __func__ << std::endl;

//    BasePacketPtr np = std::make_shared<BasePacket>();
//    np->setStatus(Status::request);
//    connectionEvent.getConnection()->send(np);
}
