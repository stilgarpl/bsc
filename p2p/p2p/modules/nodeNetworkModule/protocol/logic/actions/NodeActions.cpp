//
// Created by stilgar on 08.09.17.
//

#include <p2p/modules/nodeNetworkModule/protocol/context/NodeContext.h>
#include <p2p/modules/nodeNetworkModule/protocol/packet/NetworkInfoRequest.h>
#include <p2p/modules/nodeNetworkModule/NodeNetworkModule.h>
#include <p2p/modules/nodeNetworkModule/remote/RemoteNodeContext.h>
#include "NodeActions.h"
#include <p2p/modules/nodeNetworkModule/protocol/packet/NodeInfoGroup.h>

void NodeActions::newNodeDiscovered(const NodeInfoEvent &event) {
    Context::Ptr context = Context::getActiveContext();
    auto nodeContext = context->get<NodeContext>();
    if (nodeContext != nullptr) {
        //  NODECONTEXTLOGGER("Node discovered (connecting): " + event.getNodeInfo().getNodeId())
        auto &node = nodeContext->getNode();
        //@todo we shouldn't connect to any node, just in certain situations
//        node.getModule<NodeNetworkModule>()->connectTo(event.getNodeInfo());
    }

}

void NodeActions::updateNodeInfo(const NodeInfoEvent &event) {
    LOGGER("update node info")
    Context::Ptr context = Context::getActiveContext();
    auto remoteNodeContext = context->get<RemoteNodeContext>();
    if (remoteNodeContext) {
        auto &remoteNode = remoteNodeContext->getRemoteNode();
        remoteNode.setNodeInfo(event.getNodeInfo());
    } else {
        LOGGER("ERROR: no remote node context!")
    }
//    auto nodeContext = context->get<NodeContext>();
//    if (nodeContext != nullptr) {
//        //  NODECONTEXTLOGGER("received node info: " + event.getNodeInfo().getNodeId());
//        auto &node = nodeContext->getNode();
//        auto netModule = node.getModule<NodeNetworkModule>();
//
//    }
}

void NodeActions::addKnownNode(const NodeInfoEvent &event) {
    Context::Ptr context = Context::getActiveContext();
    auto nodeContext = context->get<NodeContext>();
    if (nodeContext != nullptr) {
        // NODECONTEXTLOGGER(                "Adding known node " + event.getNodeInfo().getNodeId() + " ... " + event.getNodeInfo().getNetworkId())
        auto &node = nodeContext->getNode();
        if (event.getNodeInfo().getNetworkId() == node.getNodeInfo().getNetworkId()) {
            node.getNetworkInfo()->addKnownNode(event.getNodeInfo());
        }

    }

}

void NodeActions::triggerUpdateNode(const Tick &tick) {
    Context::Ptr context = Context::getActiveContext();
    auto nodeContext = context->get<NodeContext>();
    //@todo imp[lement with RemoteNodes
//    if (nodeContext != nullptr) {
//        auto &node = nodeContext->getNode();
//
//        for (auto &&it :node.getModule<NodeNetworkModule>()->getClientConnections()) {
//            BasePacketPtr req = NodeInfoRequest::getNew();
//            it->connection->send(req);
//            req = std::make_shared<NetworkInfoRequest>();
//            it->connection->send(req);
//        }
//    }
}

void NodeActions::sendNetworkInfoRequest(ConnectionEvent connectionEvent) {
    LOGGER("send network info request")
    auto req = std::make_shared<NetworkInfoRequest>();
    connectionEvent.getConnection()->send(req);
}

void NodeActions::sendNodeInfoRequest(ConnectionEvent connectionEvent) {

    LOGGER("send node info request")
    NodeInfoRequest::Ptr req = NodeInfoRequest::getNew();
    connectionEvent.getConnection()->send(req);
    //        connectionEvent.context()->get<RemoteNodeContext>()->getRemoteNode().sendRequestToNode(req);
}
