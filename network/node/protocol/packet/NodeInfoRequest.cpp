//
// Created by stilgar on 08.09.17.
//

#include "NodeInfoRequest.h"
#include "../../../protocol/context/NodeContext.h"
#include "NodeInfoResponse.h"
#include "../../../protocol/context/ConnectionContext.h"

void NodeInfoRequest::process(Context &context) {
    BasePacket::process(context);

    auto nodeContext = context.get<NodeContext>();
    auto connectionContext = context.get<ConnectionContext>();

    if (nodeContext != nullptr && connectionContext != nullptr) {
        NODECONTEXTLOGGER("processing info request id" + std::to_string(this->getId()));
        auto &nodeInfo = nodeContext->getNodeInfo();
        NetworkPacketPointer<NodeInfoResponse> response = std::make_shared<NodeInfoResponse>();
        response->setNodeInfo(nodeInfo);
        response->setId(this->getId());
        connectionContext->getConnection().send(response);
    } else {
        ///@todo error level
        LOGGER("Node Context not found!");
    }
}

NodeInfoRequest::NodeInfoRequest() {
    this->setStatus(Status::REQUEST);
}
