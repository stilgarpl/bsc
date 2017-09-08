//
// Created by stilgar on 08.09.17.
//

#include "NodeSource.h"

void NodeSource::nodeInfoReceived(const NodeInfo &nodeInfo) {
    LOGGER("NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOODE IIIIIIIIIIIIIIINFOOOO");
    auto event = newEvent();
    event->setNodeInfo(nodeInfo);
    event->setEventId(EventType::IdType::NODE_INFO);

    queueEvent(event);
}
