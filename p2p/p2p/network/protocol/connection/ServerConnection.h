//
// Created by stilgar on 31.07.17.
//

#ifndef BASYCO_SERVER_CONNECTION_H
#define BASYCO_SERVER_CONNECTION_H

#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/TCPServerConnectionFactory.h>
#include <p2p/network/node/Node.h>
#include "Connection.h"
#include "IServerConnection.h"

class ServerConnection : public Poco::Net::TCPServerConnection, public IServerConnection {

private:
    Node &serverNode;

public:
    void run() override;

    ServerConnection(const Poco::Net::StreamSocket &socket, Node &serverNode, Context &context);

    void startReceiving(Poco::Net::StreamSocket &socket) override;

    void stopReceiving() override;

    virtual ~ServerConnection();

    void stop() override;
};


class ServerConnectionFactory : public Poco::Net::TCPServerConnectionFactory {
private:
    Node &serverNode;
    Context context;
public:

    ServerConnectionFactory(Node &serverNode, Context &context);

    Poco::Net::TCPServerConnection *createConnection(const Poco::Net::StreamSocket &socket) override;

};


#endif //BASYCO_SERVER_CONNECTION_H