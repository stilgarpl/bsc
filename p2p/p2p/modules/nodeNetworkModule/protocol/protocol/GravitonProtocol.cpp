//
// Created by stilgar on 06.09.17.
//

#include <p2p/node/context/NodeContext.h>
#include "GravitonProtocol.h"

void GravitonProtocol::onPacketSent(const PacketEvent &event) {
    std::lock_guard<std::mutex> g(lock);
//    NODECONTEXTLOGGER("onPacketSent" + std::to_string(event.getPacket()->getId()));
    //@todo maybe check if packet is in the response map?
}

void GravitonProtocol::onPacketReceived(const PacketEvent &event) {
    std::lock_guard<std::mutex> g(lock);
//    NODECONTEXTLOGGER("onPacketReceived" + std::to_string(event.getPacket()->getId()));
    if (responseMap.count(event.getPacket()->getId()) > 0) {
        auto &ptr = responseMap[event.getPacket()->getId()];
        if (ptr != nullptr) { //not all packets are from this protocol...
            if (event.getPacket()->getStatus() == ptr->getExpectedStatus()) {
                ptr->getResponsePromise().set_value(event.getPacket());
//                NODECONTEXTLOGGER("expected packet received")
                responseMap.erase(event.getPacket()->getId());
            } else if (event.getPacket()->getStatus() == Status::ERROR) {
                ptr->getResponsePromise().set_value(event.getPacket());
                NODECONTEXTLOGGER("ERROR HAPPENED, removing packet data, breaking promises " +
                                  std::string(typeid(event.getPacket()).name()))
                responseMap.erase(event.getPacket()->getId());
            }
        }
    }
}


void GravitonProtocol::work(const Tick &tick) {
    std::lock_guard<std::mutex> g(lock);

    auto it = std::begin(responseMap);
    while (it != std::end(responseMap)) {
        auto &value = it->second;
        if (tick.getNow() - value->getTimeSent() > MAX_TIMEOUT) {
            NODECONTEXTLOGGER("TIMEOUT REACHED, removing packet data, breaking promises")
            if (value->getRetry() >= MAX_RETRY) {
                it = responseMap.erase(it);
            } else {
                value->getConnection()->send(value->getPacketPtr());
                value->setRetry(value->getRetry() + 1);
                value->setTimeSent(tick.getNow());
                it++;
            }
        }
    }
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


//std::future<BasePacketPtr> GravitonProtocol::send(Connection *conn, BasePacketPtr p, const Status &expectedStatus) {
//    std::lock_guard<std::mutex> g(lock);
//    //  LOGGER("send");
//    std::shared_ptr<BasePacketInfo> ptr = std::make_shared<BasePacketInfo>(p, conn,
//                                                                           std::chrono::steady_clock::now(),
//                                                                           expectedStatus);
//
//    this->responseMap[p->getId()] = ptr;
//    conn->send(p);
//    return ptr->getResponsePromise().get_future();
//}

void GravitonProtocol::onConnectionEvent(const ConnectionEvent &event) {

    //remove all waiting packets for broken connections
    if (event.getEventId() == ConnectionEvent::IdType::CONNECTION_CLOSED) {
//        for (auto &&item : responseMap) {
//            item.second->getResponsePromise().
//        }

        auto item = std::begin(responseMap);
        while(item != std::end(responseMap))
            if (item->second == nullptr || item->second->getConnection() == event.getConnection()) {
                item = responseMap.erase(item);
            } else {
                item++;
        }
//        responseMap.erase(std::remove_if(responseMap.begin(),responseMap.end(),[&](auto& i)->bool{
//            return (i.second->getConnection == event.getConnection());
//        }),responseMap.end());
    }

}

GravitonProtocol::GravitonProtocol(LogicManager &logicManager) : IProtocol(logicManager) {}
