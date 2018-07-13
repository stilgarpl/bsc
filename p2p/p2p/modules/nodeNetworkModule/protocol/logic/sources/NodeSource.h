//
// Created by stilgar on 08.09.17.
//

#ifndef BASYCO_NODESOURCE_H
#define BASYCO_NODESOURCE_H


#include <p2p/modules/nodeNetworkModule/protocol/logic/events/NodeInfoEvent.h>
#include "p2p/logic/sources/EventQueueSource.h"

class NodeSource : public EventQueueSource<NodeInfoEvent> {
public:

    NodeSource(SourceManager &sourceManager);

    void nodeInfoReceived(const NodeInfo &nodeInfo);

    void nodeDiscovered(const NodeInfo &nodeInfo);
};


#endif //BASYCO_NODESOURCE_H
