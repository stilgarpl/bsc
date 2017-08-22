//
// Created by stilgar on 21.08.17.
//

#include "NodeContext.h"


NodeContext::NodeContext(Node &node, NodeInfo &nodeInfo) : node(node), nodeInfo(nodeInfo) {}

//NodeContext::NodeContext(NodeInfo &nodeInfo) : nodeInfo(nodeInfo) {}
void NodeContext::Setup::setup(Context &context) {
    auto ctx = context.get<NodeContext>();
    if (ctx == nullptr) {
        context.set<NodeContext>(std::ref(node), std::ref(nodeInfo));
    }

}

NodeContext::Setup::Setup(Node &node, NodeInfo &nodeInfo) : node(node), nodeInfo(nodeInfo) {}
