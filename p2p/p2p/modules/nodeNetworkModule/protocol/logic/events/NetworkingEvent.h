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
#include <logic/IEvent.h>
#include <p2p/modules/nodeNetworkModule/protocol/context/ConnectionContext.h>
#include <p2p/modules/nodeNetworkModule/protocol/context/ProcessorContext.h>


//template <typename idType>
//using NetworkingEvent = IEvent<idType, ConnectionOriginPointer>;
//
//

template<typename idType>
class NetworkingEvent : public IEvent<idType> {
private:
    BasePacket::IdType requestId{};

public:
    BasePacket::IdType getRequestId() const {
        return requestId;
    }

private:
    void setRequestId(BasePacket::IdType requestId) {
        NetworkingEvent::requestId = requestId;
    }

public:
    //this is required for responses unless I find a way to also set it from context
    explicit NetworkingEvent(BasePacket::IdType requestId) : requestId(requestId) {}

    NetworkingEvent() {
        LOGGER("NetworkingEvent SETUP")
        auto packet = ProcessorContext::getCurrentPacketFromActiveContext();
        if (packet != nullptr) {
            this->setRequestId(packet->getId());
        } else {
            LOGGER("ERROR: NO PACKET IN CONTEXT!")
        }
    }

};

#endif //BASYCO_NETWORKINGEVENT_H
