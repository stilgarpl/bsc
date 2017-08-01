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
