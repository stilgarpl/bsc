//
// Created by stilgar on 06.11.17.
//

#ifndef BASYCO_NETWORKINGEVENT_H
#define BASYCO_NETWORKINGEVENT_H

//template <typename idType>
//class NetworkingEvent {
//
//};


#include <p2p/modules/nodeNetworkModule/protocol/connection/Connection.h>
#include <p2p/logic/IEvent.h>
#include <p2p/modules/nodeNetworkModule/protocol/context/ConnectionContext.h>
#include <p2p/modules/nodeNetworkModule/protocol/context/ProcessorContext.h>

typedef Connection *ConnectionOriginPointer;

//@todo I'm thinking about removing setupOrigin and just make NetworkingEvent as a separate class and set it up in the constructor
///it will work the same for the rest of the lib, so it's not a problem, I can change it later
template<>
struct setupOrigin<ConnectionOriginPointer> {
    static ConnectionOriginPointer &setup(ConnectionOriginPointer &origin) {
        auto connectionContext = Context::getActiveContext()->get<ConnectionContext>();
        if (connectionContext != nullptr) {
            origin = &connectionContext->getConnection();
        } else {
            LOGGER("ERROR: NO CONNECTION CONTEXT!")
        }
        return origin;
    }
};

//template <typename idType>
//using NetworkingEvent = IEvent<idType, ConnectionOriginPointer>;
//
//

template<typename idType>
class NetworkingEvent : public IEvent<idType, ConnectionOriginPointer> {
private:
    BasePacket::IdType requestId;

public:
    BasePacket::IdType getRequestId() const {
        return requestId;
    }

    void setRequestId(BasePacket::IdType requestId) {
        NetworkingEvent::requestId = requestId;
    }

    //@todo remove this from derived classes, now that id is set from context
    NetworkingEvent(BasePacket::IdType requestId) : requestId(requestId) {}

    NetworkingEvent() = default;

    static void setup(NetworkingEvent *event) {
//        LOGGER("NetworkingEvent SETUP")
        auto packet = ProcessorContext::getCurrentPacketFromActiveContext();
        if (packet != nullptr) {
            event->setRequestId(packet->getId());
        } else {
            LOGGER("ERROR: NO PACKET IN CONTEXT!")
        }
    };
};

#endif //BASYCO_NETWORKINGEVENT_H
