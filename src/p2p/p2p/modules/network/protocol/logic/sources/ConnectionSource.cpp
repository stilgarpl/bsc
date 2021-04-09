//
// Created by Krzysztof Tulidowicz on 29.08.17.
//


#include "ConnectionSource.h"

void bsc::ConnectionSource::newConnection(bsc::Connection* con) {

    logger.debug("queue connection event");
    auto event = std::make_shared<bsc::ConnectionEvent>();
    event->setEventId(bsc::ConnectionEvent::IdType::NEW_CONNECTION);
    event->setConnection(con);
    connSource.queueEvent(event);
    // logger.debug("connection source: packet queue size " + std::to_string(packetSource.queueSize()));
}

void bsc::ConnectionSource::sentPacket(std::shared_ptr<bsc::BasePacket> p, bsc::Connection* connection) {
    // llogger.debug();

    auto event = std::make_shared<bsc::PacketEvent>();
    event->setEventId(bsc::PacketEvent::IdType::PACKET_SENT);
    event->setPacket(p);
    event->setConnection(connection);
    packetSource.queueEvent(event);
    PacketSourceWorker packetSourceWorker(*this, bsc::PacketEventId::PACKET_SENT, nullptr);
    p->_operate(packetSourceWorker, p);
}

void bsc::ConnectionSource::receivedPacket(std::shared_ptr<bsc::BasePacket> p, bsc::Connection* connection) {
    // logger.debug("received Packet " + std::to_string(p->getId()) + " " + std::to_string((int) p->getStatus()));

    auto event = std::make_shared<bsc::PacketEvent>();
    event->setEventId(bsc::PacketEvent::IdType::PACKET_RECEIVED);
    event->setPacket(p);
    event->setConnection(connection);
    packetSource.queueEvent(event);
    // logger.debug("packet source: packet queue size " + std::to_string(packetSource.queueSize()));
    PacketSourceWorker packetSourceWorker(*this, bsc::PacketEventId::PACKET_RECEIVED, connection);
    p->_operate(packetSourceWorker, p);
}

void bsc::ConnectionSource::connectionAccepted(bsc::Connection* c) {

    auto event = std::make_shared<bsc::ConnectionEvent>();
    event->setEventId(bsc::ConnectionEvent::IdType::CONNECTION_ACCEPTED);
    event->setConnection(c);
    connSource.queueEvent(event);
    newConnection(c);
}

void bsc::ConnectionSource::connectionEstablished(bsc::Connection* c) {

    auto event = std::make_shared<bsc::ConnectionEvent>();
    event->setEventId(bsc::ConnectionEvent::IdType::CONNECTION_ESTABLISHED);
    event->setConnection(c);
    connSource.queueEvent(event);
    newConnection(c);
}

void bsc::ConnectionSource::connectionClosed(bsc::Connection* c) {
    auto event = std::make_shared<bsc::ConnectionEvent>();
    event->setEventId(bsc::ConnectionEvent::IdType::CONNECTION_CLOSED);
    event->setConnection(c);
    connSource.queueEvent(event);
}

void bsc::ConnectionSource::connectionClosedServer(bsc::Connection* c) {
    auto event = std::make_shared<bsc::ConnectionEvent>();
    event->setEventId(bsc::ConnectionEvent::IdType::CONNECTION_CLOSED_CLIENT);
    event->setConnection(c);
    connSource.queueEvent(event);
}

void bsc::ConnectionSource::connectionClosedClient(bsc::Connection* c) {
    auto event = std::make_shared<bsc::ConnectionEvent>();
    event->setEventId(bsc::ConnectionEvent::IdType::CONNECTION_CLOSED_SERVER);
    event->setConnection(c);
    connSource.queueEvent(event);
}

bsc::ConnectionSource::ConnectionSource(bsc::SourceManager& sourceManager) : bsc::ISource(sourceManager),
                                                                             connSource(sourceManager),
                                                                             packetSource(sourceManager),
                                                                             advancedPacketSource(sourceManager) {}

void bsc::ConnectionSource::droppedPacket(std::shared_ptr<bsc::BasePacket> p, bsc::Connection* connection) {
    auto event = std::make_shared<bsc::PacketEvent>();
    event->setEventId(bsc::PacketEvent::IdType::PACKET_DROPPED);
    event->setPacket(p);
    event->setConnection(connection);
    packetSource.queueEvent(event);
    PacketSourceWorker packetSourceWorker(*this, bsc::PacketEventId::PACKET_DROPPED, nullptr);
    p->_operate(packetSourceWorker, p);
}

void bsc::ConnectionSource::run() {
    waitForStop();
}

void bsc::ConnectionSource::onStop() {
    packetSource.stop();
    connSource.stop();
    advancedPacketSource.stop();
    packetSource.join();
    connSource.join();
    advancedPacketSource.join();
}

void bsc::ConnectionSource::onStart() {
    packetSource.start();
    connSource.start();
    advancedPacketSource.start();
}
