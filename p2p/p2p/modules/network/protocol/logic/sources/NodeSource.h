//
// Created by stilgar on 08.09.17.
//

#ifndef BSC_NODESOURCE_H
#define BSC_NODESOURCE_H


#include <p2p/modules/network/protocol/logic/events/NodeInfoEvent.h>
#include "logic/sources/EventQueueSource.h"


namespace bsc {
    class NodeSource : public bsc::EventQueueSource<NodeInfoEvent> {
    public:

        NodeSource(SourceManager& sourceManager);

        void nodeInfoReceived(const NodeInfo& nodeInfo);

        void nodeDiscovered(const NodeInfo& nodeInfo);
    };
}


#endif //BSC_NODESOURCE_H
