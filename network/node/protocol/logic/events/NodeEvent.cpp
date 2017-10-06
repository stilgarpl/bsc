//
// Created by stilgar on 08.09.17.
//

#include "NodeEvent.h"

const NodeInfo &NodeEvent::getNodeInfo() const {
    return nodeInfo;
}

void NodeEvent::setNodeInfo(const NodeInfo &nodeInfo) {
    NodeEvent::nodeInfo = nodeInfo;
}

const NetworkInfo &NodeEvent::getNetworkInfo() const {
    return networkInfo;
}

void NodeEvent::setNetworkInfo(const NetworkInfo &networkInfo) {
    NodeEvent::networkInfo = networkInfo;
}
