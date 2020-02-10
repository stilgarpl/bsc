//
// Created by stilgar on 06.09.17.
//

#include <p2p/core/node/context/NodeContext.h>
#include "GravitonProtocol.h"

void bsc::GravitonProtocol::onPacketSent(const PacketEvent& event) {
    std::lock_guard<std::mutex> g(responseMapLock);
//    LOGGER("onPacketSent" + std::to_string(event.getPacket()->getId()));
    //@todo maybe check if packet is in the response map?
}

void bsc::GravitonProtocol::onPacketReceived(const PacketEvent& event) {
    std::lock_guard<std::mutex> g(responseMapLock);
//    LOGGER("onPacketReceived" + std::to_string(event.getPacket()->getId()));
//@todo optimize if ((auto ptr = ) != null)
    if (responseMap.contains(event.getPacket()->getId())) {
        auto& ptr = responseMap[event.getPacket()->getId()];
        if (ptr != nullptr) { //not all packets are from this protocol...
            if (event.getPacket()->getStatus() == ptr->getExpectedStatus()) {
                ptr->getResponsePromise().set_value(event.getPacket());
//                LOGGER("expected packet received")
                responseMap.erase(event.getPacket()->getId());
            } else if (event.getPacket()->getStatus() == Status::error) {
                ptr->getResponsePromise().set_value(event.getPacket());
                LOGGER("error HAPPENED, removing packet data, breaking promises " +
                       std::string(typeid(event.getPacket()).name()))
                responseMap.erase(event.getPacket()->getId());
            }
        }
    }
}


void bsc::GravitonProtocol::work(const bsc::Tick& tick) {
    std::lock_guard<std::mutex> g(responseMapLock);
//    LOGGER("gravi work " + std::to_string(responseMap.size()))
    auto it = std::begin(responseMap);
    while (it != std::end(responseMap)) {
        auto& value = it->second;
//        LOGGER("in loop")
        if (tick.getNow() - value->getTimeSent() > MAX_TIMEOUT) {
            if (value->getRetry() >= MAX_RETRY) {
                LOGGER("TIMEOUT REACHED, removing packet data, breaking promises")
                it = responseMap.erase(it);
            } else {
                value->getConnection()->send(value->getPacketPtr());
                value->setRetry(value->getRetry() + 1);
                value->setTimeSent(tick.getNow());
                it++;
            }
        } else {
            it++;
        }
    }
    //  LOGGER("onWork");
}

std::future<bsc::BasePacketPtr>
bsc::GravitonProtocol::send(Connection* conn, BasePacketPtr p, const Status& expectedStatus) {
    std::lock_guard<std::mutex> g(responseMapLock);
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

void bsc::GravitonProtocol::onConnectionEvent(const ConnectionEvent& event) {

    //remove all waiting packets for broken connections
    if (event.getEventId() == ConnectionEvent::IdType::CONNECTION_CLOSED) {
//        for (auto &&item : responseMap) {
//            item.second->getResponsePromise().
//        }

        auto item = std::begin(responseMap);
        while (item != std::end(responseMap))
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

bsc::GravitonProtocol::GravitonProtocol(bsc::LogicManager& logicManager) : IProtocol(logicManager) {}
