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

}

void ConnectionSource::sentPacket(std::shared_ptr<NetworkPacket> p, Connection *connection) {
    //lLOGGER()

    auto event = std::make_shared<PacketEvent>();
    event->setEventId(PacketEvent::IdType::PACKET_SENT);
    event->setPacket(p);
    event->setConnection(connection);
    packetSource.queueEvent(event);
}

void ConnectionSource::receivedPacket(std::shared_ptr<NetworkPacket> p, Connection *connection) {
    NODECONTEXTLOGGER("received Packet");

    auto event = std::make_shared<PacketEvent>();
    event->setEventId(PacketEvent::IdType::PACKET_RECEIVED);
    event->setPacket(p);
    event->setConnection(connection);
    packetSource.queueEvent(event);
}

void ConnectionSource::work() {

    connSource.work();
    packetSource.work();

}

void ConnectionSource::registerProviders(SourceManager *manager) {
//    ISource::registerProviders(manager);
    connSource.registerProviders(manager);
    packetSource.registerProviders(manager);
    ///@todo context settings not here...
    connSource.setContext(context);
    packetSource.setContext(context);
}

ConnectionSource::ConnectionSource() {

    //@todo ugly mapping, reimplement some other way
    connSource.assignSignal<ConnectionEvent>(
            [&, this](Context &ctx, const ConnectionEvent &connectionEvent) { this->event(connectionEvent); });
    packetSource.assignSignal<PacketEvent>(
            [&, this](Context &ctx, const PacketEvent &packetEvent) { this->event(packetEvent); });

}
