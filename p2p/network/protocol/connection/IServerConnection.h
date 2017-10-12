//
// Created by stilgar on 09.10.17.
//

#ifndef BASYCO_ISERVERCONNECTION_H
#define BASYCO_ISERVERCONNECTION_H


#include "Connection.h"

class IServerConnection : public Connection {

public:
    IServerConnection(Context &context) : Connection(context) {}

    virtual void stop() = 0;

};


#endif //BASYCO_ISERVERCONNECTION_H
