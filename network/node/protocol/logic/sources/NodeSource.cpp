//
// Created by stilgar on 08.09.17.
//

#include "NodeSource.h"

void NodeSource::nodeInfoReceived(const NodeInfo &nodeInfo) {
    auto event = newEvent();
    event->setNodeInfo(nodeInfo);
    event->setEventId(EventType::IdType::NODE_INFO_RECEIVED);

    queueEvent(event);
}
