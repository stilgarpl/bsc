//
// Created by Krzysztof Tulidowicz on 08.09.17.
//

#include "NodeInfoEvent.h"


const bsc::NodeInfo& bsc::NodeInfoEvent::getNodeInfo() const {
    return nodeInfo;
}

void bsc::NodeInfoEvent::setNodeInfo(const bsc::NodeInfo& nodeInfo) {
    NodeInfoEvent::nodeInfo = nodeInfo;
}

