//
// Created by stilgar on 20.08.17.
//

#ifndef BASYCO_CONNECTIONPROCESSOR_H
#define BASYCO_CONNECTIONPROCESSOR_H


#include <thread>
#include <p2p/modules/nodeNetworkModule/protocol/filter/PacketFilter.h>
#include <p2p/modules/nodeNetworkModule/protocol/filter/RoleFilter.h>
#include "p2p/thread/Runnable.h"
#include "p2p/context/Context.h"
#include "p2p/log/Logger.h"


class Connection;

class ConnectionProcessor : public Runnable {

private:
    Logger logger = Logger("Connection Processor");

protected:
    Connection &connection;
    //@todo initialize packet filter from AuthModule or configuration or sth.
    std::unique_ptr<PacketFilter> packetFilter = std::make_unique<RoleFilter>();

    void run() override;

public:
    explicit ConnectionProcessor(Connection &connection);

    ~ConnectionProcessor() override;

};


#endif //BASYCO_CONNECTIONPROCESSOR_H
