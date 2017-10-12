//
// Created by stilgar on 20.08.17.
//

#ifndef BASYCO_CONNECTIONPROCESSOR_H
#define BASYCO_CONNECTIONPROCESSOR_H


#include <thread>
#include "p2p/thread/Runnable.h"
#include "p2p/context/Context.h"
#include "p2p/log/Logger.h"


class Connection;

class ConnectionProcessor : public Runnable {

private:
    Logger logger = Logger("Connection Processor");

protected:
    Connection &connection;

    virtual void run();

public:
    ConnectionProcessor(Connection &connection);

    virtual ~ConnectionProcessor();

};


#endif //BASYCO_CONNECTIONPROCESSOR_H
