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

protected:
    ClientConnection(const Poco::Net::SocketAddress &a, Context::Ptr);

public:
    ClientConnection(const std::string &a, Context::Ptr);

    void startReceiving();

    void startSending();

    virtual ~ClientConnection();

    void shutdown() override;

protected:
    Poco::Net::StreamSocket &getSocket() override;

};


#endif //BASYCO_CLIENTCONNECTION_H
