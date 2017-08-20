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

ServerConnection::ServerConnection(const Poco::Net::StreamSocket &socket, Node &serverNode,
                                   const std::shared_ptr<IServerLogic> &serverLogic) : TCPServerConnection(socket),
                                                                                       serverNode(serverNode),
                                                                                       serverLogic(serverLogic) {}

void ServerConnection::startReceiving(Poco::Net::StreamSocket &socket) {
    processor.start();

}

void ServerConnection::stopReceiving() {
    Connection::stopReceiving();
}

ServerConnectionFactory::ServerConnectionFactory(Node &serverNode, const std::shared_ptr<IServerLogic> &serverLogic)
        : serverNode(serverNode), serverLogic(serverLogic) {}

Poco::Net::TCPServerConnection *ServerConnectionFactory::createConnection(const Poco::Net::StreamSocket &socket) {
    return new ServerConnection(socket, serverNode, serverLogic);
}
