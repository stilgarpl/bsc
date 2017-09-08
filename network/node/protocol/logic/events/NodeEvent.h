//
// Created by stilgar on 08.09.17.
//

#ifndef BASYCO_NODEEVENT_H
#define BASYCO_NODEEVENT_H


#include "../../../NodeInfo.h"
#include "../../../../../logic/IEvent.h"
#include "../../../Node.h"

enum class NodeEventId {
    NEW_NODE,
    NODE_CONNECTED,
    NODE_DISCONNECTED,
    NODE_INFO,
};

class NodeEvent : public IEvent<NodeEventId> {
private:
    NodeInfo nodeInfo;

public:
    const NodeInfo &getNodeInfo() const;

    void setNodeInfo(const NodeInfo &nodeInfo);
};


#endif //BASYCO_NODEEVENT_H
