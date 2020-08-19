//
// Created by Krzysztof Tulidowicz on 05.09.17.
//

#include <logic/sources/ClockSource.h>
#include <p2p/modules/network/protocol/logic/sources/ConnectionSource.h>
#include <logic/evaluators/CommonEvaluators.h>
#include "IProtocol.h"


bsc::ProtocolWrapper::ProtocolWrapper(ConnectionPtr connection, IProtocol* protocol) : connection(connection),
                                                                                       protocol(protocol) {}

std::future<bsc::BasePacketPtr> bsc::ProtocolWrapper::send(BasePacketPtr p) {
    return protocol->send(connection.get(), p);
}

void bsc::DummyProtocol::onPacketSent(const PacketEvent& event) {
    LOGGER("on packet sent")
}

void bsc::DummyProtocol::onPacketReceived(const PacketEvent& event) {
    LOGGER("on packet received")
}

void bsc::DummyProtocol::work(const bsc::Tick& tick) {
    LOGGER("on work")
}

std::future<bsc::BasePacketPtr>
bsc::DummyProtocol::send(Connection* conn, BasePacketPtr p, const Status& expectedStatus) {
    conn->send(p);
    return std::future<BasePacketPtr>();

}

void bsc::DummyProtocol::onConnectionEvent(const ConnectionEvent& event) {

}

bsc::DummyProtocol::DummyProtocol(bsc::LogicManager& logicManager) : IProtocol(logicManager) {}


void bsc::IProtocol::setupActions(bsc::ILogicModule::SetupActionHelper& actionHelper) {


}

bool bsc::IProtocol::assignActions(bsc::ILogicModule::AssignActionHelper& actionHelper) {
    bool result = true;
    when(event<PacketEvent>(PacketEvent::IdType::PACKET_SENT)).runMethod(&bsc::IProtocol::onPacketSent);
    when(event<PacketEvent>(PacketEvent::IdType::PACKET_RECEIVED)).runMethod(&IProtocol::onPacketReceived);
    when(event<bsc::Tick>(800ms)).runMethod(&IProtocol::work);
    when(event<ConnectionEvent>(ConnectionEventId::CONNECTION_CLOSED)).runMethod(&IProtocol::onConnectionEvent);

//    when(event<Tick>(800ms)).runGenericMethod(&IProtocol::testMethod, CommonEvaluators::value("PROTO"));

    return result;
}

bool bsc::IProtocol::setupSources(bsc::ILogicModule::SetupSourceHelper& sourceHelper) {
    sourceHelper.requireSource<bsc::ClockSource>();
    sourceHelper.requireSource<ConnectionSource>();
    return true;
}

bsc::IProtocol::IProtocol(bsc::LogicManager& logicManager) : bsc::LogicObject(logicManager) {}

std::future<bsc::BasePacketPtr> bsc::IProtocol::send(Connection* conn, BasePacketPtr p) {
    return this->send(conn, std::move(p), Status::response);
}

void bsc::IProtocol::testMethod(std::string a) {
    LOGGER("test method " + a);

}
