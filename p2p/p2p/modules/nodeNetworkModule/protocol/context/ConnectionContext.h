//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_CONNECTIONCONTEXTINFO_H
#define BASYCO_CONNECTIONCONTEXTINFO_H


#include <p2p/modules/nodeNetworkModule/protocol/connection/Connection.h>
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
