//
// Created by stilgar on 20.08.17.
//

#ifndef BASYCO_CONNECTIONPROCESSOR_H
#define BASYCO_CONNECTIONPROCESSOR_H


#include <thread>
#include "../../thread/Runnable.h"


class Connection;

class ConnectionProcessor : public Runnable {

protected:
    Connection &connection;

    virtual void run();

public:
    ConnectionProcessor(Connection &connection);


};


#endif //BASYCO_CONNECTIONPROCESSOR_H
