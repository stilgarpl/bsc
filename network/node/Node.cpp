//
// Created by stilgar on 31.07.17.
//

#include "Node.h"
#include "../Connection.h"

using namespace Poco::Net;

void Node::listen() {
    //SocketAddress address("127.0.0.1:6777");
    if (serverSocket == nullptr) {
        serverSocket = std::make_shared<ServerSocket>(6777);
    }
    server = std::make_shared<TCPServer>(new TCPServerConnectionFactoryImpl<Connection>, *serverSocket);
    server->start();
}


void Node::stopListening() {

}

void Node::connectTo(const NodeInfo &nodeInfo) {

}

Node::~Node() {


}
