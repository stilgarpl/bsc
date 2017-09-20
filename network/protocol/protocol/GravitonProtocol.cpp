//
// Created by stilgar on 06.09.17.
//

#include "GravitonProtocol.h"

void GravitonProtocol::onPacketSent(const PacketEvent &event) {
    LOGGER("onPacketSent");
    ///@todo maybe check if packet is in the response map?
}

void GravitonProtocol::onPacketReceived(const PacketEvent &event) {
    LOGGER("onPacketReceived");
    ///
    if (event.getPacket()->getStatus() != Status::ACK) {
        auto &ptr = responseMap[event.getPacket()->getId()];
        ptr->getResponsePromise().set_value(event.getPacket());
        responseMap.erase(event.getPacket()->getId());
    }
}

void GravitonProtocol::work(const Tick &tick) {
    LOGGER("onWork");
}

std::future<NetworkPacketPtr> GravitonProtocol::send(Connection *conn, NetworkPacketPtr p) {
    LOGGER("send");
    std::shared_ptr<NetworkPacketInfo> ptr = std::make_shared<NetworkPacketInfo>(p, conn,
                                                                                 std::chrono::steady_clock::now());
    this->responseMap[p->getId()] = ptr;
    return ptr->getResponsePromise().get_future();
}
