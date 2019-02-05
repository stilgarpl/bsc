//
// Created by stilgar on 05.02.19.
//

#ifndef BASYCO_NETWORKCONDITIONS_H
#define BASYCO_NETWORKCONDITIONS_H

#include <p2p/logic/LogicObject.h>
#include <p2p/modules/nodeNetworkModule/protocol/logic/events/PacketEvent.h>

struct NetworkConditions {


    static auto packetReceived() {
        return LogicObject::EventHelper<PacketEvent>(PacketEventId::PACKET_RECEIVED);
    }

    template<typename PacketType>
    static auto packetReceived() {
        return LogicObject::EventHelper<SpecificPacketEvent<PacketType>>(PacketEventId::PACKET_RECEIVED);
    }

    static auto packetSent() {
        return LogicObject::EventHelper<PacketEvent>(PacketEventId::PACKET_SENT);
    }

    template<typename PacketType>
    static auto packetSent() {
        return LogicObject::EventHelper<SpecificPacketEvent<PacketType>>(PacketEventId::PACKET_SENT);
    }

    static auto packetDropped() {
        return LogicObject::EventHelper<PacketEvent>(PacketEventId::PACKET_DROPPED);
    }

    template<typename PacketType>
    static auto packetDropped() {
        return LogicObject::EventHelper<SpecificPacketEvent<PacketType>>(PacketEventId::PACKET_DROPPED);
    }

};


#endif //BASYCO_NETWORKCONDITIONS_H
