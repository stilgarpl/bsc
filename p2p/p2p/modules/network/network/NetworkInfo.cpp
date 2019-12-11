//
// Created by stilgar on 31.07.17.
//


#include <p2p/modules/network/NetworkModule.h>
#include "NetworkInfo.h"
#include "p2p/node/context/NodeContext.h"
#include "logic/context/LogicContext.h"
#include "p2p/modules/network/protocol/logic/sources/NodeSource.h"



const NetworkIdType &NetworkInfo::getNetworkId() const {
    std::unique_lock g(networkInfoLock);
    return networkId;
}

void NetworkInfo::setNetworkId(const NetworkIdType &networkId) {
    std::unique_lock g(networkInfoLock);
    NetworkInfo::networkId = networkId;
}

void NetworkInfo::addKnownNode(const NodeInfo &nodeInfo) {
    std::unique_lock g(networkInfoLock);

    Context::Ptr context = Context::getActiveContext();
    auto nodeContext = context->get<NodeContext>();
    auto &node = nodeContext->getNode();
    //  LOGGER(node.getNetworkInfo()->getNetworkId());
    //  LOGGER(nodeInfo.getNetworkId());
    if (node.getModule<NetworkModule>()->getNetworkInfo()->getNetworkId() == nodeInfo.getNetworkId() &&
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
//        LOGGER("KNOWN NODES: " + it.first);
//    }
}

RemoteNodeInfo &NetworkInfo::getRemoteNodeInfo(const NodeIdType &nodeId) {
    std::unique_lock g(networkInfoLock);
    return *knownNodes[nodeId];
}

NodeInfo &NetworkInfo::getNodeInfo(const NodeIdType &nodeId) {
    std::unique_lock g(networkInfoLock);

    //@todo exception if node is not known
    return *knownNodes[nodeId]->getNodeInfo();
}

bool NetworkInfo::isNodeKnown(const NodeIdType &nodeId) {
    std::unique_lock g(networkInfoLock);

    return knownNodes.contains(nodeId);
}

NetworkInfo &NetworkInfo::operator+=(const NetworkInfo &other) {
    std::unique_lock g(networkInfoLock);

    if (networkId == other.networkId) {
        //@todo merge known nodes
        for (auto&& i : other.knownNodes) {
            if (i.second->getNodeInfo()) {
                addKnownNode(*i.second->getNodeInfo());
            }
        }
    }
    return *this;
}

void NetworkInfo::addKnownAddress(const NodeIdType &nodeId, const NetAddressType &address) {
    std::unique_lock g(networkInfoLock);


    knownNodes[nodeId]->addKnownAddress(address);

}
