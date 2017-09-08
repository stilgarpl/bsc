//
// Created by stilgar on 29.08.17.
//

#ifndef BASYCO_CONNECTIONSOURCE_H
#define BASYCO_CONNECTIONSOURCE_H


#include "../../../../logic/sources/EventQueueSource.h"
#include "../events/ConnectionEvent.h"
#include "../../connection/Connection.h"
#include "../events/PacketEvent.h"

class ConnectionSource : public ISource {

private:
    EventQueueSource<ConnectionEvent, ConnectionSource> connSource;
    EventQueueSource<PacketEvent, ConnectionSource> packetSource;
public:
    void newConnection(Connection *con);

    void sentPacket(std::shared_ptr<NetworkPacket> p, Connection *connection);

    void receivedPacket(std::shared_ptr<NetworkPacket> p, Connection *connection);

    void work() override;

    void registerProviders(SourceManager *manager) override;

    ConnectionSource();

};


#endif //BASYCO_CONNECTIONSOURCE_H
