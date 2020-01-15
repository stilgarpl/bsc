//
// Created by stilgar on 29.08.17.
//

#ifndef BASYCO_CONNECTIONSOURCE_H
#define BASYCO_CONNECTIONSOURCE_H


#include <p2p/modules/network/protocol/logic/events/ConnectionEvent.h>
#include <p2p/modules/network/protocol/logic/events/PacketEvent.h>
#include <logic/sources/EventQueueSource.h>
#include <logic/sources/AutoSource.h>


class ConnectionSource : public bsc::ISource {

private:
    bsc::EventQueueSource<ConnectionEvent, ConnectionSource> connSource;
    bsc::EventQueueSource<PacketEvent, ConnectionSource> packetSource;
    bsc::AutoSource advancedPacketSource;
public:


    explicit ConnectionSource(bsc::SourceManager& sourceManager);

    void newConnection(Connection* con);

    void sentPacket(std::shared_ptr<BasePacket> p, Connection* connection);

    void receivedPacket(std::shared_ptr<BasePacket> p, Connection* connection);

    void droppedPacket(std::shared_ptr<BasePacket> p, Connection *connection);

    void connectionAccepted(Connection *c);

    void connectionEstablished(Connection *c);

    void connectionClosed(Connection *c);

    void connectionClosedServer(Connection *c);

    void connectionClosedClient(Connection *c);

    friend class PacketSourceWorker;

private:
    //for packet source worker
    template<typename T>
    void queuePacketEvent(T &t) {
        advancedPacketSource.generateEvent<T>(t);
    }

    void run() override;

    void onStop() override;

    void onStart() override;
};


class PacketSourceWorker {
    ConnectionSource &source;
    PacketEventId eventId;
    Connection *connection;

public:
    explicit PacketSourceWorker(ConnectionSource &source, PacketEventId id, Connection *connection)
            : source(source), eventId(id), connection(connection) {}

public:
    template<typename T>
    void operate(NetworkPacketPointer<T> ptr) {
//        LOGGER(std::string("operating o ") + typeid(T).name());
        SpecificPacketEvent<T> packetEvent;
        packetEvent.setPacket(ptr);
        packetEvent.setConnection(connection);
        packetEvent.setEventId(eventId);
        source.queuePacketEvent<SpecificPacketEvent<T>>(packetEvent);

    }
};

#endif //BASYCO_CONNECTIONSOURCE_H
