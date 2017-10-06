//
// Created by stilgar on 08.09.17.
//

#ifndef BASYCO_NODEACTIONS_H
#define BASYCO_NODEACTIONS_H


#include "../../../../../context/Context.h"
#include "../../../../protocol/logic/events/ConnectionEvent.h"
#include "../../packet/NodeInfoRequest.h"
#include "../events/NodeEvent.h"
#include "../../../../protocol/context/NodeContext.h"
#include "../../packet/NetworkInfoRequest.h"
#include "../../../../../logic/events/Tick.h"
#include "../../../../protocol/context/LogicContext.h"
#include "../sources/NodeSource.h"

class NodeActions {

public:
    static void sendNodeInfoRequest(ConnectionEvent connectionEvent) {

        auto req = std::make_shared<NodeInfoRequest>();
        connectionEvent.getConnection()->send(req);
    }

    static void sendNetworkInfoRequest(ConnectionEvent connectionEvent) {

        auto req = std::make_shared<NetworkInfoRequest>();
        connectionEvent.getConnection()->send(req);
    }

    static void updateNodeInfo(const NodeEvent &event);

    static void addKnownNode(const NodeEvent &event);

    static void newNodeDiscovered(const NodeEvent &event);

    static void triggerUpdateNode(const Tick &tick) {
        Context &context = Context::getActiveContext();
        auto nodeContext = context.get<NodeContext>();
        if (nodeContext != nullptr) {
            auto &node = nodeContext->getNode();

            for (auto &&it :node.getClientConnections()) {
                NetworkPacketPtr req = std::make_shared<NodeInfoRequest>();
                it->send(req);
                req = std::make_shared<NetworkInfoRequest>();
                it->send(req);
            }
        }
    }

    static void updateNetworkInfo(const NodeEvent &event);
};


#endif //BASYCO_NODEACTIONS_H
