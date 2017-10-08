//
// Created by stilgar on 08.09.17.
//

#ifndef BASYCO_NODESOURCE_H
#define BASYCO_NODESOURCE_H


#include <network/node/protocol/logic/events/NodeInfoEvent.h>
#include "logic/sources/EventQueueSource.h"

class NodeSource : public EventQueueSource<NodeInfoEvent> {
public:

    void nodeInfoReceived(const NodeInfo &nodeInfo);

    void nodeDiscovered(const NodeInfo &nodeInfo);
};


#endif //BASYCO_NODESOURCE_H
