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

void DummyProtocol::onConnectionEvent(const ConnectionEvent &event) {

}


void IProtocol::setupActions(ILogicModule::SetupActionHelper &actionHelper) {
    actionHelper.setAction<PacketEvent>(Actions::onPacketSent, std::bind(&IProtocol::onPacketSent, this, _1));
    actionHelper.setAction<PacketEvent>(Actions::onPacketReceived,
                                        std::bind(&IProtocol::onPacketReceived, this, _1));
    actionHelper.setAction<Tick>(Actions::onWork, std::bind(&IProtocol::work, this, _1));
    actionHelper.setAction<ConnectionEvent>("protocolConnectionClosed",
                                            std::bind(&IProtocol::onConnectionEvent, this, _1));

}

bool IProtocol::assignActions(ILogicModule::AssignActionHelper &actionHelper) {
    bool result = true;
    result &= actionHelper.assignAction<PacketEvent>(PacketEvent::IdType::PACKET_SENT, Actions::onPacketSent);
    result &= actionHelper.assignAction<PacketEvent>(PacketEvent::IdType::PACKET_RECEIVED, Actions::onPacketReceived);
    result &= actionHelper.assignAction<Tick>(800ms, Actions::onWork);
    result &= actionHelper.assignAction<ConnectionEvent>(ConnectionEventId::CONNECTION_CLOSED,
                                                         "protocolConnectionClosed");
    return result;
}

bool IProtocol::setupSources(ILogicModule::SetupSourceHelper &sourceHelper) {
    sourceHelper.requireSource<ClockSource>();
    sourceHelper.requireSource<ConnectionSource>();
    return true;
}
