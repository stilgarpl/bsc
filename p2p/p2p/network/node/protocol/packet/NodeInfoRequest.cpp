//
// Created by stilgar on 08.09.17.
//

#include <p2p/network/protocol/context/NodeContext.h>
#include <p2p/network/protocol/context/ConnectionContext.h>
#include "NodeInfoResponse.h"
#include "NodeInfoRequest.h"

void NodeInfoRequest::process(Context::Ptr context) {
    BasePacket::process(context);

    auto nodeContext = context->get<NodeContext>();
    auto connectionContext = context->get<ConnectionContext>();

    if (nodeContext != nullptr && connectionContext != nullptr) {
        // NODECONTEXTLOGGER("processing info request id" + std::to_string(this->getId()));
        auto &nodeInfo = nodeContext->getNodeInfo();
        auto response = getNew<Status::RESPONSE>(this);//std::make_shared<NodeInfoResponse>();
        response->setNodeInfo(nodeInfo);
        connectionContext->getConnection().send(response);
    } else {
        ///@todo error level
        // LOGGER("Node Context not found!");
    }
}

