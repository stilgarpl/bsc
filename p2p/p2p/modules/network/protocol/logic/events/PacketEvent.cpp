//
// Created by Krzysztof Tulidowicz on 03.09.17.
//

#include "PacketEvent.h"

const bsc::BasePacketPtr& bsc::PacketEvent::getPacket() const {
    return packet;
}

void bsc::PacketEvent::setPacket(const BasePacketPtr& packet) {
    PacketEvent::packet = packet;
}

bsc::Connection* bsc::PacketEvent::getConnection() const {
    return connection;
}

void bsc::PacketEvent::setConnection(bsc::Connection* connection) {
    PacketEvent::connection = connection;
}
