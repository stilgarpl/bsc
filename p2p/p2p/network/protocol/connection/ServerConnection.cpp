//
// Created by stilgar on 31.07.17.
//

#include <thread>
#include <iostream>
#include <Poco/Net/NetException.h>
#include <p2p/network/protocol/logic/sources/ConnectionSource.h>
#include <p2p/network/protocol/context/LogicContext.h>
#include "ServerConnection.h"

using namespace std::chrono_literals;

void ServerConnection::run() {
    // std::cout << "opening server connection" << std::endl;
    // LOGGER("Opening server connection");
    //run is already in a separate thread, so there is no need to start a new one
    startSending(socket());
    workReceive(socket());

    //  std::cout << "CLOSING CONNECTION" << std::endl;

}

ServerConnection::ServerConnection(const Poco::Net::StreamSocket &socket, Node &serverNode, Context &context)
        : TCPServerConnection(
        socket),
          IServerConnection(context), serverNode(serverNode) {

    serverNode.addAcceptedConnection(this);
    auto lc = getConnectionContext().get<LogicContext>();
    auto &logicManager = lc->getLogicManager();
    auto connectionSourcePtr = logicManager.getSource<ConnectionSource>();
    connectionSourcePtr->connectionAccepted(this);
}

void ServerConnection::startReceiving(Poco::Net::StreamSocket &socket) {
    processor.start();

}

void ServerConnection::stopReceiving() {
    Connection::stopReceiving();
}

ServerConnection::~ServerConnection() {
    //  LOGGER("Server conn dest");
    serverNode.removeAcceptedConnection(this);
    stop();
}

void ServerConnection::stop() {
    //  LOGGER("stop");
    try {
        stopSending();
        stopReceiving();
        socket().shutdown();
    }
    catch (const Poco::Net::NetException &e) {
        e.displayText();
        auto nested = e.nested();
        while (nested != nullptr) {
            nested->displayText();
            nested = nested->nested();
        }
    }
}


Poco::Net::TCPServerConnection *ServerConnectionFactory::createConnection(const Poco::Net::StreamSocket &socket) {
    ServerConnection *connection = new ServerConnection(socket, serverNode, context);
    return connection;
}

ServerConnectionFactory::ServerConnectionFactory(Node &serverNode, Context &context)
        : serverNode(serverNode), context(context) {}
