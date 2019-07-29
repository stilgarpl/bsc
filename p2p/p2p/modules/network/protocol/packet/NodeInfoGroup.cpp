//
// Created by stilgar on 08.10.17.
//

#include <p2p/modules/network/protocol/packet/BasePacket.h>
#include <core/context/Context.h>
#include <p2p/node/context/NodeContext.h>
#include <p2p/modules/network/protocol/context/ConnectionContext.h>
#include "NodeInfoGroup.h"
#include <logic/context/LogicContext.h>
#include <p2p/modules/network/protocol/logic/sources/NodeSource.h>

void NodeInfoGroup::Request::process(Context::Ptr context) {
    BasePacket::process(context);

    auto nodeContext = context->get<NodeContext>();
    auto connectionContext = context->get<ConnectionContext>();

    if (nodeContext != nullptr && connectionContext != nullptr) {
        // LOGGER("processing info request id" + std::to_string(this->getId()));

        auto &nodeInfo = nodeContext->getNodeInfo();
        LOGGER("node info request... " + nodeInfo.getNodeId())
        auto response = getNew<Status::RESPONSE>(this);//std::make_shared<NodeInfoResponse>();
        response->setNodeInfo(nodeInfo);
        connectionContext->getConnection().send(response);
    } else {
        //@todo error level
        LOGGER("ERROR: Node Context not found!");
    }
}

void NodeInfoGroup::Response::process(Context::Ptr context) {
    BasePacket::process(context);

    auto lc = context->get<LogicContext>();
    if (lc != nullptr) {
        auto nodeSource = lc->getLogicManager().getSource<NodeSource>();
        nodeSource->nodeInfoReceived(this->getNodeInfo());
    }


    LOGGER("Node response received " + this->getNodeInfo().getNodeId() + "  " + std::to_string(this->getId()));
}

const NodeInfo &NodeInfoGroup::Response::getNodeInfo() const {
    return nodeInfo;
}

void NodeInfoGroup::Response::setNodeInfo(const NodeInfo &nodeInfo) {
    Response::nodeInfo = nodeInfo;
}
