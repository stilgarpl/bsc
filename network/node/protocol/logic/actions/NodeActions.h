//
// Created by stilgar on 08.09.17.
//

#ifndef BASYCO_NODEACTIONS_H
#define BASYCO_NODEACTIONS_H


#include "../../../../../context/Context.h"
#include "../../../../protocol/logic/events/ConnectionEvent.h"
#include "../../packet/NodeInfoRequest.h"
#include "../events/NodeInfoEvent.h"
#include "../../../../protocol/context/NodeContext.h"
#include "../../packet/NetworkInfoRequest.h"
#include "../../../../../logic/events/Tick.h"
#include "../../../../protocol/context/LogicContext.h"
#include "../sources/NodeSource.h"

class NodeActions {

public:
    static void sendNodeInfoRequest(ConnectionEvent connectionEvent);

    static void sendNetworkInfoRequest(ConnectionEvent connectionEvent);

    static void updateNodeInfo(const NodeInfoEvent &event);

    static void addKnownNode(const NodeInfoEvent &event);

    static void newNodeDiscovered(const NodeInfoEvent &event);

    static void triggerUpdateNode(const Tick &tick);

};


#endif //BASYCO_NODEACTIONS_H
