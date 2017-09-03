//
// Created by stilgar on 01.08.17.
//

#include "../../../context/Context.h"
#include "NetworkPacket.h"
#include "../context/LogicContext.h"
#include "../logic/sources/AuthSource.h"

void NetworkPacket::process(Context &context) {
    std::cout << "Network packet process ";


}

Status NetworkPacket::getStatus() const {
    return status;
}


void NetworkPacket::setStatus(Status status) {
    NetworkPacket::status = status;
}

NetworkPacket::IdType NetworkPacket::getId() const {
    return id;
}

void NetworkPacket::setId(NetworkPacket::IdType id) {
    NetworkPacket::id = id;
}
