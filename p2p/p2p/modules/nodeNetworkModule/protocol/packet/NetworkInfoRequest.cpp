//
// Created by stilgar on 06.10.17.
//

#include <core/context/Context.h>

#include <p2p/modules/nodeNetworkModule/protocol/packet/BasePacket.h>
#include "NetworkInfoResponse.h"
#include <p2p/node/context/NodeContext.h>
#include <p2p/modules/nodeNetworkModule/protocol/context/ConnectionContext.h>
#include <p2p/modules/nodeNetworkModule/NodeNetworkModule.h>

#include "NetworkInfoRequest.h"


void NetworkInfoRequest::process(Context::Ptr context) {
    BasePacket::process(context);

    auto nodeContext = context->get<NodeContext>();
    auto connectionContext = context->get<ConnectionContext>();

    if (nodeContext != nullptr && connectionContext != nullptr) {
        // NODECONTEXTLOGGER("processing network info request id" + std::to_string(this->getId()));
        auto &node = nodeContext->getNode();
        auto response = getNew<Status::RESPONSE>(this);//std::make_shared<NetworkInfoResponse>();
        if (node.getModule<NodeNetworkModule>()->getNetworkInfo() == nullptr) {
            NODECONTEXTLOGGER("empty network info! ")
        }
        response->setNetworkInfo(node.getModule<NodeNetworkModule>()->getNetworkInfo());
        connectionContext->getConnection().send(response);
    } else {
        //@todo error level
        LOGGER("Node Context not found!");
    }


}

