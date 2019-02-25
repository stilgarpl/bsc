//
// Created by stilgar on 05.02.19.
//

#ifndef BASYCO_NETWORKCONDITIONS_H
#define BASYCO_NETWORKCONDITIONS_H

#include <p2p/logic/LogicObject.h>
#include <p2p/modules/nodeNetworkModule/protocol/logic/events/PacketEvent.h>

struct NetworkConditions {

    static auto packet(PacketEventId id) {
        return LogicObject::event<PacketEvent>(id);
    }

    template<typename PacketType>
    static auto packet(typename SpecificPacketEvent<PacketType>::IdType id) {
        return LogicObject::event<SpecificPacketEvent<PacketType>>(id);
    }


    static auto packetReceived() {
        return packet(PacketEventId::PACKET_RECEIVED);
    }

    template<typename PacketType>
    static auto packetReceived() {
        return packet<std::decay_t<PacketType>>(PacketEventId::PACKET_RECEIVED);
    }

    static auto packetSent() {
        return packet(PacketEventId::PACKET_SENT);
    }

    template<typename PacketType>
    static auto packetSent() {
        return packet<std::decay_t<PacketType>>(PacketEventId::PACKET_SENT);
    }

    static auto packetDropped() {
        return packet(PacketEventId::PACKET_DROPPED);
    }

    template<typename PacketType>
    static auto packetDropped() {
        return packet<std::decay_t<PacketType>>(PacketEventId::PACKET_DROPPED);
    }

};


#endif //BASYCO_NETWORKCONDITIONS_H
