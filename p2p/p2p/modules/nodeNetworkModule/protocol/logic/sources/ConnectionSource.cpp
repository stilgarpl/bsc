//
// Created by stilgar on 29.08.17.
//

#include "ConnectionSource.h"

void ConnectionSource::newConnection(Connection *con) {

    std::clog << __func__ << " queue connection event" << std::endl;
    auto event = std::make_shared<ConnectionEvent>();
    event->setEventId(ConnectionEvent::IdType::NEW_CONNECTION);
    event->setConnection(con);
    connSource.queueEvent(event);
    //LOGGER("connection source: packet queue size " + std::to_string(packetSource.queueSize()))

}

void ConnectionSource::sentPacket(std::shared_ptr<BasePacket> p, Connection *connection) {
    //lLOGGER()

    auto event = std::make_shared<PacketEvent>();
    event->setEventId(PacketEvent::IdType::PACKET_SENT);
    event->setPacket(p);
    event->setConnection(connection);
    packetSource.queueEvent(event);
    PacketSourceWorker packetSourceWorker(*this, PacketEventId::PACKET_SENT);
    p->_operate(packetSourceWorker, p);
}

void ConnectionSource::receivedPacket(std::shared_ptr<BasePacket> p, Connection *connection) {
    //LOGGER("received Packet " + std::to_string(p->getId()) + " " + std::to_string((int) p->getStatus()));

    auto event = std::make_shared<PacketEvent>();
    event->setEventId(PacketEvent::IdType::PACKET_RECEIVED);
    event->setPacket(p);
    event->setConnection(connection);
    packetSource.queueEvent(event);
    // LOGGER("packet source: packet queue size " + std::to_string(packetSource.queueSize()))
    PacketSourceWorker packetSourceWorker(*this, PacketEventId::PACKET_RECEIVED);
    p->_operate(packetSourceWorker, p);
}

void ConnectionSource::work() {
    //  LOGGER("connection source WOOOOORK")
    connSource.work();
    /*   auto qs =*/ //packetSource.queueSize();
    // if (qs) LOGGER("before packet work")
    packetSource.work();
    advancedPacketSource.work();
    // if (qs) LOGGER("after packet work")


}


void ConnectionSource::connectionAccepted(Connection *c) {

    auto event = std::make_shared<ConnectionEvent>();
    event->setEventId(ConnectionEvent::IdType::CONNECTION_ACCEPTED);
    event->setConnection(c);
    connSource.queueEvent(event);
    newConnection(c);
}

void ConnectionSource::connectionEstablished(Connection *c) {

    auto event = std::make_shared<ConnectionEvent>();
    event->setEventId(ConnectionEvent::IdType::CONNECTION_ESTABLISHED);
    event->setConnection(c);
    connSource.queueEvent(event);
    newConnection(c);
}

void ConnectionSource::connectionClosed(Connection *c) {
    auto event = std::make_shared<ConnectionEvent>();
    event->setEventId(ConnectionEvent::IdType::CONNECTION_CLOSED);
    event->setConnection(c);
    connSource.queueEvent(event);
}

void ConnectionSource::connectionClosedServer(Connection *c) {
    auto event = std::make_shared<ConnectionEvent>();
    event->setEventId(ConnectionEvent::IdType::CONNECTION_CLOSED_CLIENT);
    event->setConnection(c);
    connSource.queueEvent(event);
}

void ConnectionSource::connectionClosedClient(Connection *c) {
    auto event = std::make_shared<ConnectionEvent>();
    event->setEventId(ConnectionEvent::IdType::CONNECTION_CLOSED_SERVER);
    event->setConnection(c);
    connSource.queueEvent(event);
}

ConnectionSource::ConnectionSource(SourceManager &sourceManager) : ISource(sourceManager), connSource(sourceManager),
                                                                   packetSource(sourceManager),
                                                                   advancedPacketSource(sourceManager) {}

void ConnectionSource::droppedPacket(std::shared_ptr<BasePacket> p, Connection *connection) {
    auto event = std::make_shared<PacketEvent>();
    event->setEventId(PacketEvent::IdType::PACKET_DROPPED);
    event->setPacket(p);
    event->setConnection(connection);
    packetSource.queueEvent(event);
    PacketSourceWorker packetSourceWorker(*this, PacketEventId::PACKET_DROPPED);
    p->_operate(packetSourceWorker, p);
}


