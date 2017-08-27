//
// Created by stilgar on 31.07.17.
//

#include <Poco/Net/SocketStream.h>
#include "Node.h"
#include "../protocol/connection/ServerConnection.h"
#include "../protocol/context/NodeContext.h"


using namespace Poco::Net;

void Node::listen() {
    //SocketAddress address("127.0.0.1:6777");
    if (serverSocket == nullptr) {
        unsigned short port = 6777;
        //@TODO numer portu dac z configuracji
        ////@todo sprawdzanie bledow z bindowania socketa
        if (configuration != nullptr) {
            port = configuration->getPort();
        }
        serverSocket = std::make_shared<ServerSocket>(port);
    }

    server = std::make_shared<TCPServer>(new ServerConnectionFactory(*this, this->nodeContext),
                                         *serverSocket);
    server->start();

}


void Node::stopListening() {
    server->stop();

}

bool Node::connectTo(const NodeInfo &nodeInfo) {

    for (auto i : nodeInfo.getKnownAddresses()) {

        //try to connect
        if (connectTo(i)) {

            break;
        }
    }

}

Node::~Node() {


}

bool Node::connectTo(const SocketAddress &address) {

    //std::shared_ptr<Poco::Net::StreamSocket> socket = std::make_shared<Poco::Net::StreamSocket>(address);
    //@todo check for problems and handle them
    std::shared_ptr<Connection> connection = std::make_shared<ClientConnection>(address, nodeContext);
    nodeContext.set<NodeContext>(*this, this->thisNodeInfo);
    ///@todo if connection is not connected, delete it and try another adddes
    addActiveClientConnection(connection);
    return true; ///@todo error checking

}

void Node::start() {

    listen();


}

void Node::stop() {

    stopListening();
}

void Node::addActiveClientConnection(std::shared_ptr<Connection> c) {
    activeClientConnections.push_back(c);

}

void Node::removeActiveClientConnection(std::shared_ptr<Connection> c) {
    //  auto el = std::find(activeClientConnections.begin(),activeClientConnections.end(),c);
    activeClientConnections.remove(c);

}

bool Node::connectTo(const std::string &a) {
    Poco::Net::SocketAddress address(a);
    return connectTo(address);
}

void Node::work() {

    for (auto &i : activeClientConnections) {
        //@todo implement
        //test if connection is still alive
        //process all messages from receive queue
    }

}

const std::shared_ptr<Node::Config> &Node::getConfiguration() const {
    return configuration;
}

void Node::setConfiguration(const std::shared_ptr<Node::Config> &configuration) {
    Node::configuration = configuration;
}
