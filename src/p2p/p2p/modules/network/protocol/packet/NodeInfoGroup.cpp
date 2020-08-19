//
// Created by Krzysztof Tulidowicz on 08.10.17.
//

#include <p2p/modules/network/protocol/packet/BasePacket.h>
#include <core/context/Context.h>
#include <p2p/core/node/context/NodeContext.h>
#include <p2p/modules/network/protocol/context/ConnectionContext.h>
#include "NodeInfoGroup.h"

#include <logic/context/LogicContext.h>
#include <p2p/modules/network/protocol/logic/sources/NodeSource.h>


const bsc::NodeInfo& bsc::NodeInfoGroup::Response::getNodeInfo() const {
    return nodeInfo;
}

void bsc::NodeInfoGroup::Response::setNodeInfo(const bsc::NodeInfo& nodeInfo) {
    Response::nodeInfo = nodeInfo;
}
