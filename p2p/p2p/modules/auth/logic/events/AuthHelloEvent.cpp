//
// Created by stilgar on 19.07.18.
//

#include "AuthHelloEvent.h"

const NodeIdType &AuthHelloEvent::getNodeId() const {
    return nodeId;
}

void AuthHelloEvent::setNodeId(const NodeIdType &nodeId) {
    AuthHelloEvent::nodeId = nodeId;
}

const std::string &AuthHelloEvent::getAuthData() const {
    return authData;
}

void AuthHelloEvent::setAuthData(const std::string &authData) {
    AuthHelloEvent::authData = authData;
}
