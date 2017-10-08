//
// Created by stilgar on 08.09.17.
//

#include <network/protocol/context/LogicContext.h>
#include <network/node/protocol/logic/sources/NodeSource.h>
#include "NodeInfoResponse.h"

const NodeInfo &NodeInfoResponse::getNodeInfo() const {
    return nodeInfo;
}

void NodeInfoResponse::setNodeInfo(const NodeInfo &nodeInfo) {
    NodeInfoResponse::nodeInfo = nodeInfo;
}

NodeInfoResponse::NodeInfoResponse() {
    this->setStatus(Status::RESPONSE);
}

void NodeInfoResponse::process(Context &context) {
    BasePacket::process(context);

    auto lc = context.get<LogicContext>();
    if (lc != nullptr) {
        auto nodeSource = lc->getLogicManager().getSource<NodeSource>();
        nodeSource->nodeInfoReceived(this->getNodeInfo());
    }


    LOGGER("Node response received " + std::to_string(this->getId()));
}
