//
// Created by stilgar on 06.09.17.
//

#include "GravitonProtocol.h"

void GravitonProtocol::onPacketSent(const PacketEvent &event) {
    std::lock_guard<std::mutex> g(lock);
    //  LOGGER("onPacketSent");
    ///@todo maybe check if packet is in the response map?
}

void GravitonProtocol::onPacketReceived(const PacketEvent &event) {
    std::lock_guard<std::mutex> g(lock);
    //  LOGGER("onPacketReceived");
    if (responseMap.count(event.getPacket()->getId()) > 0) {
        if (event.getPacket()->getStatus() == responseMap[event.getPacket()->getId()]->getExpectedStatus()) {
            auto &ptr = responseMap[event.getPacket()->getId()];
            if (ptr != nullptr) { //not all packets are from this protocol...
                ptr->getResponsePromise().set_value(event.getPacket());
                responseMap.erase(event.getPacket()->getId());
            }
        }
    }
}

void GravitonProtocol::work(const Tick &tick) {
    std::lock_guard<std::mutex> g(lock);
    //  LOGGER("onWork");
}

std::future<BasePacketPtr> GravitonProtocol::send(Connection *conn, BasePacketPtr p, const Status &expectedStatus) {
    std::lock_guard<std::mutex> g(lock);
    //  LOGGER("send");
    std::shared_ptr<BasePacketInfo> ptr = std::make_shared<BasePacketInfo>(p, conn,
                                                                           std::chrono::steady_clock::now(),
                                                                           expectedStatus);

    this->responseMap[p->getId()] = ptr;
    conn->send(p);
    return ptr->getResponsePromise().get_future();
}
