//
// Created by stilgar on 31.07.17.
//

#include <thread>
#include <iostream>
#include "ServerConnection.h"

using namespace std::chrono_literals;

void ServerConnection::run() {
    std::cout << "opening server connection" << std::endl;
    //run is already in a separate thread, so there is no need to start a new one
    startSending(socket());
    workReceive(socket());

    std::cout << "CLOSING CONNECTION";

}

ServerConnection::ServerConnection(const Poco::Net::StreamSocket &socket, Node &serverNode) : TCPServerConnection(
        socket),
                                                                                              serverNode(serverNode) {}

void ServerConnection::startReceiving(Poco::Net::StreamSocket &socket) {
    processor.start();

}

void ServerConnection::stopReceiving() {
    Connection::stopReceiving();
}

ServerConnectionFactory::ServerConnectionFactory(Node &serverNode)
        : serverNode(serverNode) {}

Poco::Net::TCPServerConnection *ServerConnectionFactory::createConnection(const Poco::Net::StreamSocket &socket) {
    ServerConnection *connection = new ServerConnection(socket, serverNode);
    if (contextSetup != nullptr) {
        contextSetup->setup(connection->getProcessor().getContext());
    }
    return connection;
}

const std::shared_ptr<IContextSetup> &ServerConnectionFactory::getContextSetup() const {
    return contextSetup;
}

void ServerConnectionFactory::setContextSetup(const std::shared_ptr<IContextSetup> &contextSetup) {
    ServerConnectionFactory::contextSetup = contextSetup;
}

ServerConnectionFactory::ServerConnectionFactory(Node &serverNode, const std::shared_ptr<IContextSetup> &contextSetup)
        : serverNode(serverNode), contextSetup(contextSetup) {}
