//
// Created by Krzysztof Tulidowicz on 31.07.17.
//

#include "NodeInfo.h"


const std::string& bsc::NodeInfo::getNetworkId() const {
    return networkId;
}

const bsc::NodeIdType& bsc::NodeInfo::getNodeId() const {
    return nodeId;
}

void bsc::NodeInfo::setNodeId(const NodeIdType& nodeId) {
    NodeInfo::nodeId = nodeId;
}

void bsc::NodeInfo::setNetworkId(const NetworkIdType& networkId) {
    NodeInfo::networkId = networkId;
}

//Node::IdType NodeInfo::getId() const {
//    return id;
//}
//
//void NodeInfo::setId(Node::IdType id) {
//    NodeInfo::id = id;
//}
