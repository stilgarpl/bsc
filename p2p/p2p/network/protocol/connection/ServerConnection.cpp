//
// Created by stilgar on 31.07.17.
//

#include <thread>
#include <iostream>
#include <utility>
#include <Poco/Net/NetException.h>
#include <p2p/network/protocol/logic/sources/ConnectionSource.h>
#include <p2p/network/protocol/context/LogicContext.h>
#include <p2p/network/node/modules/NodeNetworkModule.h>
#include "ServerConnection.h"

using namespace std::chrono_literals;

void ServerConnection::run() {
    // std::cout << "opening server connection" << std::endl;
    // LOGGER("Opening server connection");
    //run is already in a separate thread, so there is no need to start a new one
    socket().setReceiveTimeout(Poco::Timespan(5, 1));
    socket().setSendTimeout(Poco::Timespan(5, 1));
    socket().setKeepAlive(true);
    startSending(socket());
    workReceive(socket());

    //  std::cout << "CLOSING CONNECTION" << std::endl;

}

ServerConnection::ServerConnection(const Poco::Net::StreamSocket &socket, Node &serverNode, Context::Ptr context)
        : TCPServerConnection(
        socket),
          IServerConnection(std::move(context)), serverNode(serverNode) {

    Context::setActiveContext(getConnectionContext());
    ///@todo observer pattern?
    serverNode.getModule<NodeNetworkModule>()->addAcceptedConnection(this);
    auto lc = getConnectionContext()->get<LogicContext>();
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
    ///@todo remove this, we have an event for this now -- this would also remove ServerConnection dependency on Node. it would make things much simpler
    serverNode.getModule<NodeNetworkModule>()->removeAcceptedConnection(this);
    stop();
}

void ServerConnection::stop() {
    //  LOGGER("stop");
    shutdown();
}

void ServerConnection::shutdown() {
    Connection::shutdown();
    try {
        socket().shutdown();
//        auto lc = getConnectionContext().get<LogicContext>();
//        auto &logicManager = lc->getLogicManager();
//        auto connectionSourcePtr = logicManager.getSource<ConnectionSource>();
//        connectionSourcePtr->connectionClosedServer(this);
    }
    catch (const Poco::Net::NetException &e) {
        LOGGER("net exception")
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

ServerConnectionFactory::ServerConnectionFactory(Node &serverNode, Context::Ptr context)
        : serverNode(serverNode), context(Context::makeContext(context)) {}
