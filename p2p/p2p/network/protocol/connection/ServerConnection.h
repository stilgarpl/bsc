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

    ServerConnection(const Poco::Net::StreamSocket &socket, Node &serverNode, Context::Ptr context);

    void startReceiving(Poco::Net::StreamSocket &socket) override;

    void stopReceiving() override;

    virtual ~ServerConnection();

    void stop() override;

    void shutdown() override;
};


class ServerConnectionFactory : public Poco::Net::TCPServerConnectionFactory {
private:
    Node &serverNode;
    Context::OwnPtr context = nullptr; //initialized from the constructor
public:

    ServerConnectionFactory(Node &serverNode, Context::Ptr context);

    Poco::Net::TCPServerConnection *createConnection(const Poco::Net::StreamSocket &socket) override;

};


#endif //BASYCO_SERVER_CONNECTION_H
