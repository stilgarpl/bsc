#include <utility>

#include <utility>

//
// Created by stilgar on 31.07.17.
//

#include <thread>
#include <iostream>
#include <utility>
#include <Poco/Net/NetException.h>
#include <p2p/modules/network/protocol/logic/sources/ConnectionSource.h>
#include <logic/context/LogicContext.h>
#include <p2p/modules/network/NetworkModule.h>
#include "ServerConnection.h"


using namespace std::chrono_literals;

void ServerConnection::run() {
    // std::cout << "opening server connection" << std::endl;
    // LOGGER("Opening server connection");
    //run is already in a separate thread, so there is no need to start a new one
    bsc::Context::setActiveContext(getConnectionContext());
//    socket().setReceiveTimeout(Poco::Timespan(150, 1));
//    socket().setSendTimeout(Poco::Timespan(150, 1));
//    socket().setKeepAlive(true);
    startSending(socket());
    processor.start();
    changeState(ConnectionState::CONNECTED);
    try {
        workReceive(socket());
    } catch (const Poco::Net::NetException& e) {
        LOGGER("Server connection caught poco net exception")
        //processor.stop();
        stopReceiving();
        stopSending();
        e.displayText();

    }

    processor.stop();
    stopSending();
    changeState(ConnectionState::DISCONNECTED);
    std::cout << "CLOSING CONNECTION" << std::endl;

}

ServerConnection::ServerConnection(const Poco::Net::StreamSocket& socket, bsc::Context::Ptr context)
        : TCPServerConnection(
        socket),
          IServerConnection(std::move(context)) {

    bsc::Context::setActiveContext(getConnectionContext());
    auto& lc = getConnectionContext()->get<LogicContext>();
    auto &logicManager = lc.getLogicManager();
    auto connectionSourcePtr = logicManager.getSource<ConnectionSource>();
    connectionSourcePtr->connectionAccepted(this);
}

void ServerConnection::startReceiving(Poco::Net::StreamSocket &socket) {


}

void ServerConnection::stopReceiving() {
    Connection::stopReceiving();
}

ServerConnection::~ServerConnection() {
    //  LOGGER("Server conn dest");
    changeState(ConnectionState::DISCONNECTED);
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
//        auto &logicManager = lc.getLogicManager();
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

Poco::Net::StreamSocket &ServerConnection::getSocket() {
    return socket();
}


Poco::Net::TCPServerConnection *ServerConnectionFactory::createConnection(const Poco::Net::StreamSocket &socket) {
    LOGGER("creating server connection");
    bsc::Context::Ptr connectionContext = contextGetter();
    bsc::SetLocalContext localContext(connectionContext); //RAII
    ServerConnection* connection = new ServerConnection(socket, connectionContext);
    for (const auto& observer : observers) {
        connection->registerStateObserver(observer);
    }
    return connection;
}

ServerConnectionFactory::ServerConnectionFactory(std::function<bsc::Context::OwnPtr(void)> contextGetter,
                                                 std::list<std::reference_wrapper<Connection::ObserverType>> observers)
        : contextGetter(std::move(contextGetter)), observers(std::move(observers)) {}
