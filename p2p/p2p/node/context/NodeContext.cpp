//
// Created by stilgar on 21.08.17.
//


#include "NodeContext.h"

bsc::NodeContext::NodeContext(bsc::Node& node, bsc::NodeInfo& nodeInfo) : node(node), nodeInfo(nodeInfo) {}

bsc::Node& bsc::NodeContext::getNode() const {
    return node;
}

bsc::NodeInfo& bsc::NodeContext::getNodeInfo() const {
    return nodeInfo;
}

