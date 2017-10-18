//
// Created by stilgar on 29.08.17.
//

#ifndef BASYCO_CONNECTIONSOURCE_H
#define BASYCO_CONNECTIONSOURCE_H


#include <p2p/network/protocol/logic/events/ConnectionEvent.h>
#include "p2p/logic/sources/EventQueueSource.h"

class ConnectionSource : public ISource {

private:
    EventQueueSource<ConnectionEvent, ConnectionSource> connSource;
    EventQueueSource<PacketEvent, ConnectionSource> packetSource;
public:
    void newConnection(Connection *con);

    void sentPacket(std::shared_ptr<BasePacket> p, Connection *connection);

    void receivedPacket(std::shared_ptr<BasePacket> p, Connection *connection);

    void work() override;

    void registerProviders(SourceManager *manager) override;

    void connectionAccepted(Connection *c);

    ConnectionSource();

    void connectionEstablished(Connection *c);
};


#endif //BASYCO_CONNECTIONSOURCE_H