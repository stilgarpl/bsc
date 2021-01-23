//
// Created by Krzysztof Tulidowicz on 01.08.17.
//

#include "BasePacket.h"
#include "context/context/Context.h"
#include <atomic>

bsc::Status bsc::BasePacket::getStatus() const {
    return status;
}


void bsc::BasePacket::setStatus(Status status) {
    bsc::BasePacket::status = status;
}

bsc::BasePacket::IdType bsc::BasePacket::getId() const {
    return id;
}

void bsc::BasePacket::setId(BasePacket::IdType id) {
    BasePacket::id = id;
}

bool bsc::BasePacket::isRetry() const {
    return retry;
}

void bsc::BasePacket::setRetry(bool retry) {
    BasePacket::retry = retry;
}

bsc::BasePacket::IdType bsc::BasePacket::nextId() {
    std::lock_guard<std::mutex> g(idLock);
    static std::atomic<IdType> val = 0;
    return val++;
}
