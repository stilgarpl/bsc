//
// Created by stilgar on 31.07.17.
//


#include <p2p/modules/nodeNetworkModule/NodeNetworkModule.h>
#include "NetworkInfo.h"
#include "p2p/modules/nodeNetworkModule/protocol/context/NodeContext.h"
#include "p2p/modules/nodeNetworkModule/protocol/context/LogicContext.h"
#include "p2p/modules/nodeNetworkModule/protocol/logic/sources/NodeSource.h"



const NetworkIdType &NetworkInfo::getNetworkId() const {
    return networkId;
}

void NetworkInfo::setNetworkId(const NetworkIdType &networkId) {
    NetworkInfo::networkId = networkId;
}

void NetworkInfo::addKnownNode(const NodeInfo &nodeInfo) {

    Context::Ptr context = Context::getActiveContext();
    auto nodeContext = context->get<NodeContext>();
    auto &node = nodeContext->getNode();
    //  NODECONTEXTLOGGER(node.getNetworkInfo()->getNetworkId());
    //  NODECONTEXTLOGGER(nodeInfo.getNetworkId());
    if (node.getModule<NodeNetworkModule>()->getNetworkInfo()->getNetworkId() == nodeInfo.getNetworkId() &&
        !isNodeKnown(nodeInfo.getNodeId())) {

        auto lc = context->get<LogicContext>();
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
