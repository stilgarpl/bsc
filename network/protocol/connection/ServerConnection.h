//
// Created by stilgar on 31.07.17.
//

#ifndef BASYCO_SERVER_CONNECTION_H
#define BASYCO_SERVER_CONNECTION_H

#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/TCPServerConnectionFactory.h>
#include "../../node/Node.h"
#include "../logic/IServerLogic.h"

class ServerConnection : public Poco::Net::TCPServerConnection, public Connection {

private:
    Node &serverNode;
    std::shared_ptr<IServerLogic> serverLogic;

public:
    void run() override;

    ServerConnection(const Poco::Net::StreamSocket &socket, Node &serverNode,
                     const std::shared_ptr<IServerLogic> &serverLogic);

    void startReceiving(Poco::Net::StreamSocket &socket) override;

    void stopReceiving() override;
};


class ServerConnectionFactory : public Poco::Net::TCPServerConnectionFactory {
private:
    Node &serverNode;
    std::shared_ptr<IServerLogic> serverLogic;
public:
    ServerConnectionFactory(Node &serverNode, const std::shared_ptr<IServerLogic> &serverLogic);

    Poco::Net::TCPServerConnection *createConnection(const Poco::Net::StreamSocket &socket) override;


};


#endif //BASYCO_SERVER_CONNECTION_H
