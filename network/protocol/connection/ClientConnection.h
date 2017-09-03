//
// Created by stilgar on 01.08.17.
//

#ifndef BASYCO_CLIENTCONNECTION_H
#define BASYCO_CLIENTCONNECTION_H


#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include "Connection.h"

class ClientConnection : public Connection {

private:
    Poco::Net::StreamSocket socket;

public:
    ClientConnection(const Poco::Net::SocketAddress &a, Context &);

    void startReceiving();

    void startSending();


};


#endif //BASYCO_CLIENTCONNECTION_H
