//
// Created by stilgar on 31.07.17.
//

#ifndef BASYCO_NODE_H
#define BASYCO_NODE_H


#include "NodeInfo.h"
#include <Poco/Net/ServerSocket.h>
#include <memory>
#include <Poco/Net/TCPServer.h>

class Node {
private:
    std::shared_ptr<Poco::Net::ServerSocket> serverSocket;
    std::shared_ptr<Poco::Net::TCPServer> server;
public:
    void listen();

    void stopListening();

    void connectTo(const NodeInfo &nodeInfo);

    virtual ~Node();
};


#endif //BASYCO_NODE_H
