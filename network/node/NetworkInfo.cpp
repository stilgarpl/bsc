//
// Created by stilgar on 31.07.17.
//


#include "NetworkInfo.h"
#include "../protocol/context/NodeContext.h"
#include "../protocol/context/LogicContext.h"
#include "protocol/logic/sources/NodeSource.h"


const NetworkIdType &NetworkInfo::getNetworkId() const {
    return networkId;
}

void NetworkInfo::setNetworkId(const NetworkIdType &networkId) {
    NetworkInfo::networkId = networkId;
}

void NetworkInfo::addKnownNode(const NodeInfo &nodeInfo) {

    Context &context = Context::getActiveContext();
    //auto nodeContext = context.get<NodeContext>();
    if (!isNodeKnown(nodeInfo.getNodeId())) {

        auto lc = context.get<LogicContext>();
        if (lc != nullptr) {
            lc->getLogicManager().getSource<NodeSource>()->nodeDiscovered(nodeInfo);

        }
    }

    knownNodes[nodeInfo.getNodeId()] = nodeInfo;

    for (auto &&node : knownNodes) {
        NODECONTEXTLOGGER("KNOWN NODES: " + node.first);
    }
}
