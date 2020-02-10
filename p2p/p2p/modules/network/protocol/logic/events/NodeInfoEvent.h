//
// Created by stilgar on 08.09.17.
//

#ifndef BSC_NODEEVENT_H
#define BSC_NODEEVENT_H


#include <p2p/core/node/NodeInfo.h>
#include "logic/IEvent.h"


namespace bsc {
    enum class NodeEventId {
        NEW_NODE_DISCOVERED,
        NODE_CONNECTED,
        NODE_DISCONNECTED,
        NODE_INFO_RECEIVED,
    };

    class NodeInfoEvent : public bsc::IEvent<NodeEventId> {
    private:
        NodeInfo nodeInfo;


    public:
        const NodeInfo& getNodeInfo() const;

        void setNodeInfo(const NodeInfo& nodeInfo);


    };


}


#endif //BSC_NODEEVENT_H
