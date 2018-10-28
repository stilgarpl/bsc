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

    //@todo too many make shareds. do something with it! it's ugly
    auto newNode = std::make_shared<RemoteNodeInfo>();
    newNode->setNodeInfo(std::make_shared<NodeInfo>(nodeInfo));
    knownNodes[nodeInfo.getNodeId()] = newNode;
//
//    for (auto &&it : knownNodes) {
//        NODECONTEXTLOGGER("KNOWN NODES: " + it.first);
//    }
}

RemoteNodeInfo &NetworkInfo::getRemoteNodeInfo(const NodeIdType &nodeId) {
    return *knownNodes[nodeId];
}

NodeInfo &NetworkInfo::getNodeInfo(const NodeIdType &nodeId) {
    //@todo exception if node is not known
    return *knownNodes[nodeId]->getNodeInfo();
}

bool NetworkInfo::isNodeKnown(const NodeIdType &nodeId) {
    return knownNodes.count(nodeId) > 0;
}

NetworkInfo &NetworkInfo::operator+=(const NetworkInfo &other) {
    if (networkId == other.networkId) {
        //@todo merge known nodes
        for (auto &&i : other.knownNodes) {
            if (i.second->getNodeInfo()) {
                addKnownNode(*i.second->getNodeInfo());
            }
        }
    }
    return *this;
}

void NetworkInfo::addKnownAddress(const NodeIdType &nodeId, const NetAddressType &address) {

    knownNodes[nodeId]->addKnownAddress(address);

}
