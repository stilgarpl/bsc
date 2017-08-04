//
// Created by stilgar on 31.07.17.
//

#include "NodeInfo.h"

const std::string &NodeInfo::getNodeName() const {
    return nodeName;
}

void NodeInfo::setNodeName(const std::string &nodeName) {
    NodeInfo::nodeName = nodeName;
}

const std::string &NodeInfo::getNetworkId() const {
    return networkId;
}

const std::list<std::string> &NodeInfo::getKnownAddresses() const {
    return knownAddresses;
}
