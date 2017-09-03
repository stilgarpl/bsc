//
// Created by stilgar on 03.09.17.
//

#include "PacketEvent.h"

const NetworkPacketPtr &PacketEvent::getPacket() const {
    return packet;
}

void PacketEvent::setPacket(const NetworkPacketPtr &packet) {
    PacketEvent::packet = packet;
}

Connection *PacketEvent::getConnection() const {
    return connection;
}

void PacketEvent::setConnection(Connection *connection) {
    PacketEvent::connection = connection;
}
