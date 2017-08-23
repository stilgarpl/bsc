//
// Created by stilgar on 20.08.17.
//

#ifndef BASYCO_CONNECTIONPROCESSOR_H
#define BASYCO_CONNECTIONPROCESSOR_H


#include <thread>
#include "../../../thread/Runnable.h"
#include "../../../context/Context.h"
#include "../../../log/Logger.h"


class Connection;

class ConnectionProcessor : public Runnable {

private:
    Logger logger = Logger("Connection Processor");

protected:
    Context context;
    Connection &connection;

    virtual void run();

public:
    ConnectionProcessor(Connection &connection);


    Context &getContext();
};


#endif //BASYCO_CONNECTIONPROCESSOR_H
