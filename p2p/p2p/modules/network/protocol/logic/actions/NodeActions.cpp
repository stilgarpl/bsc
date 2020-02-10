//
// Created by stilgar on 08.09.17.
//

#include <p2p/node/context/NodeContext.h>
#include <p2p/modules/network/protocol/packet/NetworkInfoRequest.h>
#include <p2p/modules/network/NetworkModule.h>
#include <p2p/modules/network/remote/RemoteNodeContext.h>
#include "NodeActions.h"
#include <p2p/modules/network/NetworkModule.h>
#include <p2p/modules/network/protocol/packet/NodeInfoGroup.h>
#include <p2p/modules/network/protocol/connection/ConnectionException.h>

void bsc::NodeActions::newNodeDiscovered(const NodeInfoEvent& event) {
    Context::Ptr context = Context::getActiveContext();
//    auto &nodeContext = context->get<NodeContext>();

    //  LOGGER("Node discovered (connecting): " + event.getNodeInfo().getNodeId())
//        auto &node = nodeContext->getNode();
    //@todo we shouldn't connect to any node, just in certain situations
//        node.getModule<NetworkModule>()->connectTo(event.getNodeInfo());


}

void bsc::NodeActions::updateNodeInfo(const NodeInfoEvent& event) {
    LOGGER("update node info")
    Context::Ptr context = Context::getActiveContext();
    auto remoteNodeContext = context->get<RemoteNodeContext>();

    auto& remoteNode = remoteNodeContext->getRemoteNode();
    remoteNode.setNodeInfo(event.getNodeInfo());

//    auto nodeContext = context->get<NodeContext>();
//    if (nodeContext != nullptr) {
//        //  LOGGER("received node info: " + event.getNodeInfo().getNodeId());
//        auto &node = nodeContext->getNode();
//        auto netModule = node.getModule<NetworkModule>();
//
//    }
}

void bsc::NodeActions::addKnownNode(const NodeInfoEvent& event) {
    Context::Ptr context = Context::getActiveContext();
    auto nodeContext = context->get<NodeContext>();

    // LOGGER(                "Adding known node " + event.getNodeInfo().getNodeId() + " ... " + event.getNodeInfo().getNetworkId())
    auto& node = nodeContext->getNode();
    if (event.getNodeInfo().getNetworkId() == node.getNodeInfo().getNetworkId()) {
        node.getModule<NetworkModule>()->getNetworkInfo()->addKnownNode(event.getNodeInfo());

        //@todo do it better way, this is quick and dirty
        auto remoteNodeContext = context->get<RemoteNodeContext>();

        auto& remoteNode = remoteNodeContext->getRemoteNode();
        try {
            if (remoteNode.getAddress()) {
                node.getModule<NetworkModule>()->getNetworkInfo()->addKnownAddress(
                        event.getNodeInfo().getNodeId(),
                        *remoteNode.getAddress());

            }
        } catch (const bsc::ConnectionException& e) {
            ERROR("Error while adding known node.")
        }


    }


}

void bsc::NodeActions::triggerUpdateNode(const bsc::Tick& tick) {
    Context::Ptr context = Context::getActiveContext();
//    auto nodeContext = context->get<NodeContext>();
    //@todo imp[lement with RemoteNodes
//    if (nodeContext != nullptr) {
//        auto &node = nodeContext->getNode();
//
//        for (auto &&it :node.getModule<NetworkModule>()->getClientConnections()) {
//            BasePacketPtr req = NodeInfoRequest::getNew();
//            it->connection->send(req);
//            req = std::make_shared<NetworkInfoRequest>();
//            it->connection->send(req);
//        }
//    }
}

void bsc::NodeActions::sendNetworkInfoRequest(ConnectionEvent connectionEvent) {
    LOGGER("send network info request")
    auto req = std::make_shared<bsc::NetworkInfoRequest>();
    connectionEvent.getConnection()->send(req);
}

void bsc::NodeActions::sendNodeInfoRequest(ConnectionEvent connectionEvent) {

    LOGGER("send node info request")
    NodeInfoRequest::Ptr req = NodeInfoRequest::getNew();
    connectionEvent.getConnection()->send(req);
    //        connectionEvent.context()->get<RemoteNodeContext>()->getRemoteNode().sendRequestToNode(req);
}
