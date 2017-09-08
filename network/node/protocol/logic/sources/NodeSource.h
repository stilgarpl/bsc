//
// Created by stilgar on 08.09.17.
//

#ifndef BASYCO_NODESOURCE_H
#define BASYCO_NODESOURCE_H


#include "../../../../../logic/sources/EventQueueSource.h"
#include "../events/NodeEvent.h"

class NodeSource : public EventQueueSource<NodeEvent> {
public:

    void nodeInfoReceived(const NodeInfo &nodeInfo);
};


#endif //BASYCO_NODESOURCE_H
