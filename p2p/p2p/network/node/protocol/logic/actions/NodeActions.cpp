//
// Created by stilgar on 08.09.17.
//

#include <p2p/network/protocol/context/NodeContext.h>
#include <p2p/network/node/protocol/packet/NodeInfoRequest.h>
#include <p2p/network/node/protocol/packet/NetworkInfoRequest.h>
#include <p2p/network/node/modules/NodeNetworkModule.h>
#include "NodeActions.h"


void NodeActions::newNodeDiscovered(const NodeInfoEvent &event) {
    Context &context = Context::getActiveContext();
    auto nodeContext = context.get<NodeContext>();
    if (nodeContext != nullptr) {
        //  NODECONTEXTLOGGER("Node discovered (connecting): " + event.getNodeInfo().getNodeId())
        auto &node = nodeContext->getNode();
        ///@todo we shouldn't connect to any node, just in certain situations
//        node.getModule<NodeNetworkModule>()->connectTo(event.getNodeInfo());
    }

}

void NodeActions::updateNodeInfo(const NodeInfoEvent &event) {
    Context &context = Context::getActiveContext();
    auto nodeContext = context.get<NodeContext>();
    if (nodeContext != nullptr) {
        //  NODECONTEXTLOGGER("received node info: " + event.getNodeInfo().getNodeId());
    }
}

void NodeActions::addKnownNode(const NodeInfoEvent &event) {
    Context &context = Context::getActiveContext();
    auto nodeContext = context.get<NodeContext>();
    if (nodeContext != nullptr) {
        // NODECONTEXTLOGGER(                "Adding known node " + event.getNodeInfo().getNodeId() + " ... " + event.getNodeInfo().getNetworkId())
        auto &node = nodeContext->getNode();
        if (event.getNodeInfo().getNetworkId() == node.getNodeInfo().getNetworkId()) {
            node.getNetworkInfo()->addKnownNode(event.getNodeInfo());
        }

    }

}

void NodeActions::triggerUpdateNode(const Tick &tick) {
    Context &context = Context::getActiveContext();
    auto nodeContext = context.get<NodeContext>();
    if (nodeContext != nullptr) {
        auto &node = nodeContext->getNode();

        for (auto &&it :node.getModule<NodeNetworkModule>()->getClientConnections()) {
            BasePacketPtr req = NodeInfoRequest::getNew();
            it->connection->send(req);
            req = std::make_shared<NetworkInfoRequest>();
            it->connection->send(req);
        }
    }
}

void NodeActions::sendNetworkInfoRequest(ConnectionEvent connectionEvent) {

    auto req = std::make_shared<NetworkInfoRequest>();
    connectionEvent.getConnection()->send(req);
}

void NodeActions::sendNodeInfoRequest(ConnectionEvent connectionEvent) {

    auto req = std::make_shared<NodeInfoRequest>();
    connectionEvent.getConnection()->send(req);
}
