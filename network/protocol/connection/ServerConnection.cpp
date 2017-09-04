//
// Created by stilgar on 31.07.17.
//

#include <thread>
#include <iostream>
#include "ServerConnection.h"

using namespace std::chrono_literals;

void ServerConnection::run() {
    // std::cout << "opening server connection" << std::endl;
    LOGGER("Opening server connection");
    //run is already in a separate thread, so there is no need to start a new one
    startSending(socket());
    workReceive(socket());

    //  std::cout << "CLOSING CONNECTION";

}

ServerConnection::ServerConnection(const Poco::Net::StreamSocket &socket, Node &serverNode, Context &context)
        : TCPServerConnection(
        socket),
          serverNode(serverNode),
          Connection(context) {}

void ServerConnection::startReceiving(Poco::Net::StreamSocket &socket) {
    processor.start();

}

void ServerConnection::stopReceiving() {
    Connection::stopReceiving();
}


Poco::Net::TCPServerConnection *ServerConnectionFactory::createConnection(const Poco::Net::StreamSocket &socket) {
    ServerConnection *connection = new ServerConnection(socket, serverNode, context);
    return connection;
}

ServerConnectionFactory::ServerConnectionFactory(Node &serverNode, Context &context)
        : serverNode(serverNode), context(context) {}
