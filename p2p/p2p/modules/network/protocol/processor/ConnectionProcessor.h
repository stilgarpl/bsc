//
// Created by stilgar on 20.08.17.
//

#ifndef BASYCO_CONNECTIONPROCESSOR_H
#define BASYCO_CONNECTIONPROCESSOR_H


#include <thread>
#include <p2p/modules/network/protocol/filter/PacketFilter.h>
#include <p2p/modules/network/protocol/filter/RoleFilter.h>
#include "core/thread/Runnable.h"
#include "core/context/Context.h"
#include "core/log/Logger.h"


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

    ~ConnectionProcessor() = default;

};


#endif //BASYCO_CONNECTIONPROCESSOR_H
