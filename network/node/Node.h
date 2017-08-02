//
// Created by stilgar on 31.07.17.
//

#ifndef BASYCO_NODE_H
#define BASYCO_NODE_H


#include "NodeInfo.h"
#include "../protocol/ClientConnection.h"
#include "NetworkInfo.h"
#include <Poco/Net/ServerSocket.h>
#include <memory>
#include <Poco/Net/TCPServer.h>

class Node {
private:
    std::shared_ptr<Poco::Net::ServerSocket> serverSocket;
    std::shared_ptr<Poco::Net::TCPServer> server;
    NodeInfo thisNodeInfo;
    std::shared_ptr<NetworkInfo> networkInfo; //network this node belongs to @todo more than 1?
public:
    void listen();

    void stopListening();

    void connectTo(const NodeInfo &nodeInfo);

    ClientConnection connectTo(const Poco::Net::SocketAddress &address);


    virtual ~Node();
};


#endif //BASYCO_NODE_H
