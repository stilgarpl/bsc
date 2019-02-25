//
// Created by stilgar on 29.08.17.
//

#ifndef BASYCO_CONNECTIONSOURCE_H
#define BASYCO_CONNECTIONSOURCE_H


#include <p2p/modules/nodeNetworkModule/protocol/logic/events/ConnectionEvent.h>
#include <p2p/modules/nodeNetworkModule/protocol/logic/events/PacketEvent.h>
#include <p2p/logic/sources/EventQueueSource.h>
#include <p2p/logic/sources/AutoSource.h>

class ConnectionSource : public ISource {

private:
    EventQueueSource<ConnectionEvent, ConnectionSource> connSource;
    EventQueueSource<PacketEvent, ConnectionSource> packetSource;
    AutoSource advancedPacketSource;
public:


    explicit ConnectionSource(SourceManager &sourceManager);

    void newConnection(Connection *con);

    void sentPacket(std::shared_ptr<BasePacket> p, Connection *connection);

    void receivedPacket(std::shared_ptr<BasePacket> p, Connection *connection);

    void droppedPacket(std::shared_ptr<BasePacket> p, Connection *connection);

    void work() override;

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
};


class PacketSourceWorker {
    ConnectionSource &source;
    PacketEventId eventId;

public:
    explicit PacketSourceWorker(ConnectionSource &source, PacketEventId id) : source(source), eventId(id) {}

public:
    template<typename T>
    void operate(NetworkPacketPointer<T> ptr) {
//        LOGGER(std::string("operating o ") + typeid(T).name());
        SpecificPacketEvent<T> packetEvent;
        packetEvent.setPacket(ptr);
        packetEvent.setEventId(eventId);
        source.queuePacketEvent<SpecificPacketEvent<T>>(packetEvent);

    }
};

#endif //BASYCO_CONNECTIONSOURCE_H
