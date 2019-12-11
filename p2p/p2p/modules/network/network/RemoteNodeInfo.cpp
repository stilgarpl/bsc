//
// Created by stilgar on 24.10.18.
//

#include "RemoteNodeInfo.h"

const std::set<std::string> &RemoteNodeInfo::getKnownAddresses() const {
    std::unique_lock g(nodeInfoLock);
    return knownAddresses;
}

std::shared_ptr<NodeInfo> RemoteNodeInfo::getNodeInfo() const {
    std::unique_lock g(nodeInfoLock);
    //return nodeInfo.load();
    return std::atomic_load(&nodeInfo);
}

void RemoteNodeInfo::setNodeInfo(const std::shared_ptr<NodeInfo> &nodeInfo) {
    std::unique_lock g(nodeInfoLock);
    //RemoteNodeInfo::nodeInfo.store(nodeInfo);
    std::atomic_store(&this->nodeInfo, nodeInfo);
}

