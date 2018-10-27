//
// Created by stilgar on 31.07.17.
//

#include "NodeInfo.h"


const std::string &NodeInfo::getNetworkId() const {
    return networkId;
}

const NodeIdType &NodeInfo::getNodeId() const {
    return nodeId;
}

void NodeInfo::setNodeId(const NodeIdType &nodeId) {
    NodeInfo::nodeId = nodeId;
}

void NodeInfo::setNetworkId(const NetworkIdType &networkId) {
    NodeInfo::networkId = networkId;
}

//Node::IdType NodeInfo::getId() const {
//    return id;
//}
//
//void NodeInfo::setId(Node::IdType id) {
//    NodeInfo::id = id;
//}
