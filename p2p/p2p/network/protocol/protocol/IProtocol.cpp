//
// Created by stilgar on 05.09.17.
//

#include <p2p/logic/sources/ClockSource.h>
#include <p2p/network/protocol/logic/sources/ConnectionSource.h>
#include "IProtocol.h"


ProtocolWrapper::ProtocolWrapper(ConnectionPtr connection, IProtocol *protocol) : connection(connection),
                                                                                  protocol(protocol) {}

std::future<BasePacketPtr> ProtocolWrapper::send(BasePacketPtr p) {
    //return protocol->send(connection,p);
    return std::future<BasePacketPtr>();
}

void DummyProtocol::onPacketSent(const PacketEvent &event) {
    LOGGER("on packet sent")
}

void DummyProtocol::onPacketReceived(const PacketEvent &event) {
    LOGGER("on packet received")
}

void DummyProtocol::work(const Tick &tick) {
    LOGGER("on work")
}

std::future<BasePacketPtr> DummyProtocol::send(Connection *conn, BasePacketPtr p, const Status &expectedStatus) {
    conn->send(p);
    return std::future<BasePacketPtr>();

}


void IProtocol::setupActions(LogicManager &logicManager) {
    logicManager.setAction<PacketEvent>(Actions::onPacketSent, std::bind(&IProtocol::onPacketSent, this, _1));
    logicManager.setAction<PacketEvent>(Actions::onPacketReceived,
                                        std::bind(&IProtocol::onPacketReceived, this, _1));
    logicManager.setAction<Tick>(Actions::onWork, std::bind(&IProtocol::work, this, _1));

}

bool IProtocol::assignActions(LogicManager &logicManager) {
    bool result = true;
    result &= logicManager.assignAction<PacketEvent>(PacketEvent::IdType::PACKET_SENT, Actions::onPacketSent);
    result &= logicManager.assignAction<PacketEvent>(PacketEvent::IdType::PACKET_RECEIVED, Actions::onPacketReceived);
    result &= logicManager.assignAction<Tick>(800ms, Actions::onWork);
    return result;
}

bool IProtocol::setupSources(LogicManager &logicManager) {
    logicManager.requireSource<ClockSource>();
    logicManager.requireSource<ConnectionSource>();
    return true;
}
