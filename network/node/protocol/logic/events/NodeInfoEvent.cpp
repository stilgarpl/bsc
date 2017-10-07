//
// Created by stilgar on 08.09.17.
//

#include "NodeInfoEvent.h"

const NodeInfo &NodeInfoEvent::getNodeInfo() const {
    return nodeInfo;
}

void NodeInfoEvent::setNodeInfo(const NodeInfo &nodeInfo) {
    NodeInfoEvent::nodeInfo = nodeInfo;
}

