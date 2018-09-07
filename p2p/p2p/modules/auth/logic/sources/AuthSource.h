//
// Created by stilgar on 19.07.18.
//

#ifndef BASYCO_AUTHSOURCE_H
#define BASYCO_AUTHSOURCE_H


#include <p2p/node/NodeInfo.h>
#include <p2p/logic/sources/EventQueueSource.h>
#include <p2p/modules/auth/logic/events/AuthHelloEvent.h>

class AuthSource : public EventQueueSource<AuthHelloEvent> {


public:

    explicit AuthSource(SourceManager &sourceManager) : EventQueueSource(sourceManager) {}

    void hello(const NodeIdType &nodeId, const std::string &authData) {
        auto event = newEvent();
        event->setNodeId(nodeId);
        event->setAuthData(authData);
        queueEvent(event);
    }
};


#endif //BASYCO_AUTHSOURCE_H
