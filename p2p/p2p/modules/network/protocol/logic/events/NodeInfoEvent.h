//
// Created by stilgar on 08.09.17.
//

#ifndef BASYCO_NODEEVENT_H
#define BASYCO_NODEEVENT_H


#include <p2p/node/NodeInfo.h>
#include "logic/IEvent.h"



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


#endif //BASYCO_NODEEVENT_H
