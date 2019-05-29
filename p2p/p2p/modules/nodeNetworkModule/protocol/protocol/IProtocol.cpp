//
// Created by stilgar on 05.09.17.
//

#include <p2p/logic/sources/ClockSource.h>
#include <p2p/modules/nodeNetworkModule/protocol/logic/sources/ConnectionSource.h>
#include "IProtocol.h"


ProtocolWrapper::ProtocolWrapper(ConnectionPtr connection, IProtocol *protocol) : connection(connection),
                                                                                  protocol(protocol) {}

std::future<BasePacketPtr> ProtocolWrapper::send(BasePacketPtr p) {
    return protocol->send(connection.get(),p);
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

DummyProtocol::DummyProtocol(LogicManager &logicManager) : IProtocol(logicManager) {}


void IProtocol::setupActions(ILogicModule::SetupActionHelper &actionHelper) {


}

bool IProtocol::assignActions(ILogicModule::AssignActionHelper &actionHelper) {
    bool result = true;
    when(event<PacketEvent>(PacketEvent::IdType::PACKET_SENT)).runMethod(&IProtocol::onPacketSent);
    when(event<PacketEvent>(PacketEvent::IdType::PACKET_RECEIVED)).runMethod(&IProtocol::onPacketReceived);
    when(event<Tick>(800ms)).runMethod(this, &IProtocol::work);
    when(event<ConnectionEvent>(ConnectionEventId::CONNECTION_CLOSED)).runMethod(&IProtocol::onConnectionEvent);

    return result;
}

bool IProtocol::setupSources(ILogicModule::SetupSourceHelper &sourceHelper) {
    sourceHelper.requireSource<ClockSource>();
    sourceHelper.requireSource<ConnectionSource>();
    return true;
}

IProtocol::IProtocol(LogicManager &logicManager) : LogicObject(logicManager) {}

std::future<BasePacketPtr> IProtocol::send(Connection *conn, BasePacketPtr p) {
    return this->send(conn, std::move(p),Status::RESPONSE);
}
