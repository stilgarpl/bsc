//
// Created by stilgar on 06.09.17.
//

#include "TransmissionControl.h"

NetworkPacketInfo::NetworkPacketInfo(const NetworkPacketPtr &packetPtr,
                                     const std::chrono::time_point<Tick::clock> &timeSent)
        : packetPtr(packetPtr), timeSent(timeSent) {}

const NetworkPacketPtr &NetworkPacketInfo::getPacketPtr() const {
    return packetPtr;
}

void NetworkPacketInfo::setPacketPtr(const NetworkPacketPtr &packetPtr) {
    NetworkPacketInfo::packetPtr = packetPtr;
}

const std::chrono::time_point<Tick::clock> &NetworkPacketInfo::getTimeSent() const {
    return timeSent;
}

void NetworkPacketInfo::setTimeSent(const std::chrono::time_point<Tick::clock> &timeSent) {
    NetworkPacketInfo::timeSent = timeSent;
}

NetworkPacketInfo::NetworkPacketInfo(const NetworkPacketPtr &packetPtr, Connection *connection,
                                     const std::chrono::time_point<Tick::clock> &timeSent) : packetPtr(packetPtr),
                                                                                             connection(connection),
                                                                                             timeSent(timeSent) {}

Connection *NetworkPacketInfo::getConnection() const {
    return connection;
}

void NetworkPacketInfo::setConnection(Connection *connection) {
    NetworkPacketInfo::connection = connection;
}

std::promise<NetworkPacketPtr> &NetworkPacketInfo::getResponsePromise() {
    return responsePromise;
}


