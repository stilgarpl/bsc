//
// Created by stilgar on 31.07.17.
//


#include "NetworkInfo.h"
#include "p2p/network/protocol/context/NodeContext.h"
#include "p2p/network/protocol/context/LogicContext.h"
#include "p2p/network/node/protocol/logic/sources/NodeSource.h"


const NetworkIdType &NetworkInfo::getNetworkId() const {
    return networkId;
}

void NetworkInfo::setNetworkId(const NetworkIdType &networkId) {
    NetworkInfo::networkId = networkId;
}

void NetworkInfo::addKnownNode(const NodeInfo &nodeInfo) {

    Context &context = Context::getActiveContext();
    auto nodeContext = context.get<NodeContext>();
    auto &node = nodeContext->getNode();
    //  NODECONTEXTLOGGER(node.getNetworkInfo()->getNetworkId());
    //  NODECONTEXTLOGGER(nodeInfo.getNetworkId());
    if (node.getNetworkInfo()->getNetworkId() == nodeInfo.getNetworkId() && !isNodeKnown(nodeInfo.getNodeId())) {

        auto lc = context.get<LogicContext>();
        if (lc != nullptr) {
            lc->getLogicManager().getSource<NodeSource>()->nodeDiscovered(nodeInfo);

        }
    }


    knownNodes[nodeInfo.getNodeId()] = nodeInfo;
//
//    for (auto &&it : knownNodes) {
//        NODECONTEXTLOGGER("KNOWN NODES: " + it.first);
//    }
}
