//
// Created by Krzysztof Tulidowicz on 06.09.17.
//
#include "BasePacketInfo.h"


bsc::BasePacketInfo::BasePacketInfo(const BasePacketPtr& packetPtr,
                                    const std::chrono::time_point<bsc::Tick::Clock>& timeSent)
        : packetPtr(packetPtr), timeSent(timeSent) {}

const bsc::BasePacketPtr& bsc::BasePacketInfo::getPacketPtr() const {
    return packetPtr;
}

void bsc::BasePacketInfo::setPacketPtr(const BasePacketPtr& packetPtr) {
    BasePacketInfo::packetPtr = packetPtr;
}

const std::chrono::time_point<bsc::Tick::Clock>& bsc::BasePacketInfo::getTimeSent() const {
    return timeSent;
}

void bsc::BasePacketInfo::setTimeSent(const std::chrono::time_point<bsc::Tick::Clock>& timeSent) {
    BasePacketInfo::timeSent = timeSent;
}

bsc::BasePacketInfo::BasePacketInfo(const BasePacketPtr& packetPtr, bsc::Connection* connection,
                                    const std::chrono::time_point<bsc::Tick::Clock>& timeSent,
                                    bsc::Status expectedStatus)
        : packetPtr(packetPtr),
          connection(connection), expectedStatus(expectedStatus),
          timeSent(timeSent) {}

bsc::Connection* bsc::BasePacketInfo::getConnection() const {
    return connection;
}

void bsc::BasePacketInfo::setConnection(bsc::Connection* connection) {
    BasePacketInfo::connection = connection;
}

std::promise<bsc::BasePacketPtr>& bsc::BasePacketInfo::getResponsePromise() {
    return responsePromise;
}

bsc::Status bsc::BasePacketInfo::getExpectedStatus() const {
    return expectedStatus;
}

void bsc::BasePacketInfo::setExpectedStatus(bsc::Status expectedStatus) {
    bsc::BasePacketInfo::expectedStatus = expectedStatus;
}

bsc::BasePacketInfo::BasePacketInfo(const BasePacketPtr& packetPtr, bsc::Connection* connection,
                                    const std::chrono::time_point<bsc::Tick::Clock>& timeSent) : packetPtr(packetPtr),
                                                                                                 connection(connection),
                                                                                                 timeSent(timeSent) {}

unsigned int bsc::BasePacketInfo::getRetry() const {
    return retry;
}

void bsc::BasePacketInfo::setRetry(unsigned int retry) {
    BasePacketInfo::retry = retry;
}


