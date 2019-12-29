//
// Created by stilgar on 08.10.17.
//

#include <p2p/modules/network/protocol/packet/BasePacket.h>
#include <core/context/Context.h>
#include <p2p/node/context/NodeContext.h>
#include <p2p/modules/network/protocol/context/ConnectionContext.h>
#include "NodeInfoGroup.h"
#include <logic/context/LogicContext.h>
#include <p2p/modules/network/protocol/logic/sources/NodeSource.h>


const NodeInfo &NodeInfoGroup::Response::getNodeInfo() const {
    return nodeInfo;
}

void NodeInfoGroup::Response::setNodeInfo(const NodeInfo &nodeInfo) {
    Response::nodeInfo = nodeInfo;
}
