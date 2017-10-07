//
// Created by stilgar on 08.09.17.
//

#include "NodeActions.h"


void NodeActions::newNodeDiscovered(const NodeInfoEvent &event) {
    Context &context = Context::getActiveContext();
    auto nodeContext = context.get<NodeContext>();
    if (nodeContext != nullptr) {
        NODECONTEXTLOGGER("Node discovered (connecting): " + event.getNodeInfo().getNodeId())
        auto &node = nodeContext->getNode();
        node.connectTo(event.getNodeInfo());
    }

}

void NodeActions::updateNodeInfo(const NodeInfoEvent &event) {
    Context &context = Context::getActiveContext();
    auto nodeContext = context.get<NodeContext>();
    if (nodeContext != nullptr) {
        //  NODECONTEXTLOGGER("received node info: " + event.getNodeInfo().getNodeId());
    }
}

void NodeActions::updateNetworkInfo(const NetworkInfoEvent &event) {
    Context &context = Context::getActiveContext();
    auto nodeContext = context.get<NodeContext>();
    if (nodeContext != nullptr) {
        auto &node = nodeContext->getNode();
        if (node.getNetworkInfo() != nullptr &&
            node.getNetworkInfo()->getNetworkId() == event.getNetworkInfo().getNetworkId()) {
            //     LOGGER("SAME NETWORK! " + event.getNetworkInfo().getNetworkId());
            *node.getNetworkInfo() += event.getNetworkInfo();

        }
        // NODECONTEXTLOGGER("received network info: " + event.getNetworkInfo().getNetworkId());
    }
}

void NodeActions::addKnownNode(const NodeInfoEvent &event) {
    Context &context = Context::getActiveContext();
    auto nodeContext = context.get<NodeContext>();
    if (nodeContext != nullptr) {
        LOGGER("Adding known node " + event.getNodeInfo().getNodeId() + " ... " + event.getNodeInfo().getNetworkId())
        auto &node = nodeContext->getNode();
        if (event.getNodeInfo().getNetworkId() == node.getNodeInfo().getNetworkId()) {
            node.getNetworkInfo()->addKnownNode(event.getNodeInfo());
        }

    }

}
