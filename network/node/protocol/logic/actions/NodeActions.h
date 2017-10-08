//
// Created by stilgar on 08.09.17.
//

#ifndef BASYCO_NODEACTIONS_H
#define BASYCO_NODEACTIONS_H


#include <network/protocol/logic/events/ConnectionEvent.h>
#include <network/node/protocol/logic/events/NodeInfoEvent.h>
#include <logic/events/Tick.h>
#include "context/Context.h"

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
