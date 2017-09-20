//
// Created by stilgar on 29.08.17.
//

#include "ProtocolActions.h"

void ProtocolActions::onNewConnection(const ConnectionEvent &connectionEvent) {

    std::clog << __func__ << std::endl;

    NetworkPacketPtr np = std::make_shared<NetworkPacket>();
    np->setStatus(Status::REQUEST);
    connectionEvent.getConnection()->send(np);
}
