//
// Created by stilgar on 19.07.18.
//

#ifndef BASYCO_AUTHHELLOEVENT_H
#define BASYCO_AUTHHELLOEVENT_H


#include <p2p/node/NodeInfo.h>
#include <logic/IEvent.h>

class AuthHelloEvent : public IEvent<int> {
    NodeIdType nodeId;
    std::string authData;

public:
    const NodeIdType &getNodeId() const;

    void setNodeId(const NodeIdType &nodeId);

    const std::string &getAuthData() const;

    void setAuthData(const std::string &authData);
};


#endif //BASYCO_AUTHHELLOEVENT_H
