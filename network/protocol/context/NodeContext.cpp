//
// Created by stilgar on 21.08.17.
//

#include "NodeContext.h"


NodeContext::NodeContext(Node &node, NodeInfo &nodeInfo) : node(node), nodeInfo(nodeInfo) {}

Node &NodeContext::getNode() const {
    return node;
}

NodeInfo &NodeContext::getNodeInfo() const {
    return nodeInfo;
}

