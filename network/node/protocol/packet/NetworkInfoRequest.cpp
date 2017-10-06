//
// Created by stilgar on 06.10.17.
//

#include "NetworkInfoRequest.h"
#include "NodeInfoResponse.h"
#include "../../../protocol/context/ConnectionContext.h"
#include "../../../protocol/context/NodeContext.h"
#include "NetworkInfoResponse.h"

void NetworkInfoRequest::process(Context &context) {
    BasePacket::process(context);

    auto nodeContext = context.get<NodeContext>();
    auto connectionContext = context.get<ConnectionContext>();

    if (nodeContext != nullptr && connectionContext != nullptr) {
        NODECONTEXTLOGGER("processing network info request id" + std::to_string(this->getId()));
        auto &node = nodeContext->getNode();
        auto response = std::make_shared<NetworkInfoResponse>();
        response->setId(this->getId());
        if (node.getNetworkInfo() == nullptr) {
            NODECONTEXTLOGGER("empty network info! ")
        }
        response->setNetworkInfo(node.getNetworkInfo());
        connectionContext->getConnection().send(response);
    } else {
        ///@todo error level
        LOGGER("Node Context not found!");
    }


}

NetworkInfoRequest::NetworkInfoRequest() {
    this->setStatus(Status::REQUEST);
}
