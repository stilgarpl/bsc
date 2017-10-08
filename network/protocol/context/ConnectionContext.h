//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_CONNECTIONCONTEXTINFO_H
#define BASYCO_CONNECTIONCONTEXTINFO_H


#include <network/protocol/connection/Connection.h>
#include "IContextSetup.h"

class ConnectionContext {
public:

private:
    Connection &connection;


public:
    explicit ConnectionContext(Connection &connection);

    Connection &getConnection() const;

    int test;


};


#endif //BASYCO_CONNECTIONCONTEXTINFO_H
