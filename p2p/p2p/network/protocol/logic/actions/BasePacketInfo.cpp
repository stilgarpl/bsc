//
// Created by stilgar on 06.09.17.
//

#include "TransmissionControl.h"

BasePacketInfo::BasePacketInfo(const BasePacketPtr &packetPtr,
                               const std::chrono::time_point<Tick::clock> &timeSent)
        : packetPtr(packetPtr), timeSent(timeSent) {}

const BasePacketPtr &BasePacketInfo::getPacketPtr() const {
    return packetPtr;
}

void BasePacketInfo::setPacketPtr(const BasePacketPtr &packetPtr) {
    BasePacketInfo::packetPtr = packetPtr;
}

const std::chrono::time_point<Tick::clock> &BasePacketInfo::getTimeSent() const {
    return timeSent;
}

void BasePacketInfo::setTimeSent(const std::chrono::time_point<Tick::clock> &timeSent) {
    BasePacketInfo::timeSent = timeSent;
}

BasePacketInfo::BasePacketInfo(const BasePacketPtr &packetPtr, Connection *connection,
                                     const std::chrono::time_point<Tick::clock> &timeSent) : packetPtr(packetPtr),
                                                                                             connection(connection),
                                                                                             timeSent(timeSent) {}

Connection *BasePacketInfo::getConnection() const {
    return connection;
}

void BasePacketInfo::setConnection(Connection *connection) {
    BasePacketInfo::connection = connection;
}

std::promise<BasePacketPtr> &BasePacketInfo::getResponsePromise() {
    return responsePromise;
}


