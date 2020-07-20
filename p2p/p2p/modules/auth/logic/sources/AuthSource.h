//
// Created by Krzysztof Tulidowicz on 19.07.18.
//

#ifndef BSC_AUTHSOURCE_H
#define BSC_AUTHSOURCE_H


#include <p2p/core/node/NodeInfo.h>
#include <logic/sources/EventQueueSource.h>
#include <p2p/modules/auth/logic/events/AuthHelloEvent.h>


namespace bsc {
    class AuthSource : public EventQueueSource<AuthHelloEvent> {


    public:

        explicit AuthSource(SourceManager& sourceManager) : EventQueueSource(sourceManager) {}

        void hello(const NodeIdType& nodeId, const std::string& authData) {
            auto event = newEvent();
            event->setNodeId(nodeId);
            event->setAuthData(authData);
            queueEvent(event);
        }
    };
}


#endif //BSC_AUTHSOURCE_H
