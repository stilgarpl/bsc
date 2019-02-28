//
// Created by stilgar on 03.09.17.
//

#ifndef BASYCO_PACKETEVENT_H
#define BASYCO_PACKETEVENT_H

#include <p2p/modules/nodeNetworkModule/protocol/packet/BasePacket.h>
#include <p2p/modules/nodeNetworkModule/protocol/connection/Connection.h>
#include "p2p/logic/IEvent.h"

enum class PacketEventId {
    PACKET_SENT,
    PACKET_RECEIVED,
    PACKET_DROPPED,
};

class PacketEvent : public IEvent<PacketEventId> {
private:
    //@todo maybe merge with ConnectionEvent?
    BasePacketPtr packet;
    Connection *connection;

public:
    const BasePacketPtr &getPacket() const;

    void setPacket(const BasePacketPtr &packet);

    Connection *getConnection() const;

    void setConnection(Connection *connection);

};

template<typename PacketType>
class SpecificPacketEvent : public IEvent<PacketEventId> {
private:
    NetworkPacketPointer<PacketType> packet;
    Connection *connection;

public:
    const NetworkPacketPointer<PacketType> &getPacket() const {
        return packet;
    }

    void setPacket(const NetworkPacketPointer<PacketType> &packet) {
        SpecificPacketEvent::packet = packet;
    }

    Connection *getConnection() const {
        return connection;
    }

    void setConnection(Connection *connection) {
        SpecificPacketEvent::connection = connection;
    }
};


#endif //BASYCO_PACKETEVENT_H
