//
// Created by stilgar on 08.09.17.
//

#include "NodeInfoResponse.h"
#include "../../../protocol/context/LogicContext.h"
#include "../logic/sources/NodeSource.h"

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
    NetworkPacket::process(context);

    auto lc = context.get<LogicContext>();
    if (lc != nullptr) {
        auto nodeSource = lc->getLogicManager().getSource<NodeSource>();
        nodeSource->nodeInfoReceived(this->getNodeInfo());
    }


    LOGGER("Node response received" + std::to_string(this->getId()));
}
