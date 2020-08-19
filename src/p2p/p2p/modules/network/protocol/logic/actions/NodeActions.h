//
// Created by Krzysztof Tulidowicz on 08.09.17.
//

#ifndef BSC_NODEACTIONS_H
#define BSC_NODEACTIONS_H


#include <p2p/modules/network/protocol/logic/events/ConnectionEvent.h>
#include <p2p/modules/network/protocol/logic/events/NodeInfoEvent.h>
#include <logic/events/Tick.h>
#include "core/context/Context.h"


namespace bsc {
    class NodeActions {

    public:
        static void sendNodeInfoRequest(ConnectionEvent connectionEvent);

        static void sendNetworkInfoRequest(ConnectionEvent connectionEvent);

        static void updateNodeInfo(const NodeInfoEvent& event);

        static void addKnownNode(const NodeInfoEvent& event);

        static void newNodeDiscovered(const NodeInfoEvent& event);

        static void triggerUpdateNode(const Tick& tick);

    };
}


#endif //BSC_NODEACTIONS_H
