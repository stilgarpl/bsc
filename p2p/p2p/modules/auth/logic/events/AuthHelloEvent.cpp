//
// Created by stilgar on 19.07.18.
//

#include "AuthHelloEvent.h"

const bsc::NodeIdType& bsc::AuthHelloEvent::getNodeId() const {
    return nodeId;
}

void bsc::AuthHelloEvent::setNodeId(const NodeIdType& nodeId) {
    AuthHelloEvent::nodeId = nodeId;
}

const std::string& bsc::AuthHelloEvent::getAuthData() const {
    return authData;
}

void bsc::AuthHelloEvent::setAuthData(const std::string& authData) {
    AuthHelloEvent::authData = authData;
}
