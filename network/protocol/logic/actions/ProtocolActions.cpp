//
// Created by stilgar on 29.08.17.
//

#include "ProtocolActions.h"

void ProtocolActions::onNewConnection(const ConnectionEvent &connectionEvent) {

    std::clog << __func__ << std::endl;

    BasePacketPtr np = std::make_shared<BasePacket>();
    np->setStatus(Status::REQUEST);
    connectionEvent.getConnection()->send(np);
}
