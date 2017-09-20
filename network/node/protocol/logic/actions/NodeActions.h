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

class NodeActions {

public:
    static void sendNodeInfoRequest(ConnectionEvent connectionEvent) {
        //    LOGGER("NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOODE IIIIIIIIIIIIIIINFOOOO");

        NetworkPacketPointer<NodeInfoRequest> req = std::make_shared<NodeInfoRequest>();
        connectionEvent.getConnection()->send(req);
    }

    static void updateNodeInfo(const NodeEvent &event) {
        Context &context = Context::getActiveContext();
        auto nodeContext = context.get<NodeContext>();
        if (nodeContext != nullptr) {
            LOGGER("received node info: " + event.getNodeInfo().getNodeName());
        }
    }
};


#endif //BASYCO_NODEACTIONS_H
