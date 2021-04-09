//
// Created by Krzysztof Tulidowicz on 31.07.17.
//


#include <p2p/modules/network/NetworkModule.h>

#include "p2p/core/node/context/NodeContext.h"
#include "logic/context/LogicContext.h"
#include "p2p/modules/network/protocol/logic/sources/NodeSource.h"



const bsc::NetworkIdType& bsc::NetworkInfo::getNetworkId() const {
    std::unique_lock g(networkInfoLock);
    return networkId;
}

void bsc::NetworkInfo::setNetworkId(const NetworkIdType& networkId) {
    std::unique_lock g(networkInfoLock);
    NetworkInfo::networkId = networkId;
}

void bsc::NetworkInfo::addKnownNode(const bsc::NodeInfo& nodeInfo) {
    std::unique_lock g(networkInfoLock);

    Context::Ptr context = Context::getActiveContext();
    auto nodeContext     = context->get<NodeContext>();
    auto& node           = nodeContext->getNode();
    //  logger.debug(node.getNetworkInfo()->getNetworkId());
    //  logger.debug(nodeInfo.getNetworkId());
    if (node.getModule<NetworkModule>()->getNetworkInfo()->getNetworkId() == nodeInfo.getNetworkId() &&
        !isNodeKnown(nodeInfo.getNodeId())) {

        auto lc = context->get<bsc::LogicContext>();
        lc->getLogicManager().getSource<bsc::NodeSource>()->nodeDiscovered(nodeInfo);
    }

    //@todo too many make shareds. do something with it! it's ugly
    auto newNode = std::make_shared<RemoteNodeInfo>();
    newNode->setNodeInfo(std::make_shared<bsc::NodeInfo>(nodeInfo));
    knownNodes[nodeInfo.getNodeId()] = newNode;
//
    //    for (auto &&it : knownNodes) {
    //        logger.debug("KNOWN NODES: " + it.first);
    //    }
}

bsc::RemoteNodeInfo& bsc::NetworkInfo::getRemoteNodeInfo(const NodeIdType& nodeId) {
    std::unique_lock g(networkInfoLock);
    return *knownNodes[nodeId];
}

bsc::NodeInfo& bsc::NetworkInfo::getNodeInfo(const NodeIdType& nodeId) {
    std::unique_lock g(networkInfoLock);

    //@todo exception if node is not known
    return *knownNodes[nodeId]->getNodeInfo();
}

bool bsc::NetworkInfo::isNodeKnown(const NodeIdType& nodeId) {
    std::unique_lock g(networkInfoLock);

    return knownNodes.contains(nodeId);
}

bsc::NetworkInfo& bsc::NetworkInfo::operator+=(const bsc::NetworkInfo& other) {
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

void bsc::NetworkInfo::addKnownAddress(const NodeIdType& nodeId, const bsc::NetAddressType& address) {
    std::unique_lock g(networkInfoLock);


    knownNodes[nodeId]->addKnownAddress(address);

}
