//
// Created by stilgar on 01.08.17.
//

#include "p2p/context/Context.h"
#include "BasePacket.h"

void BasePacket::process(Context::Ptr context) {
    // std::cout << "Network packet process ";


}

Status BasePacket::getStatus() const {
    return status;
}


void BasePacket::setStatus(Status status) {
    BasePacket::status = status;
}

BasePacket::IdType BasePacket::getId() const {
    return id;
}

void BasePacket::setId(BasePacket::IdType id) {
    BasePacket::id = id;
}

bool BasePacket::isRetry() const {
    return retry;
}

void BasePacket::setRetry(bool retry) {
    BasePacket::retry = retry;
}

BasePacket::IdType BasePacket::nextId() {
    std::lock_guard<std::mutex> g(idLock);
    static IdType val = 0;
    return val++;
}
