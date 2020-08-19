#include <utility>

//
// Created by Krzysztof Tulidowicz on 01.08.17.
//

#include <Poco/Net/SocketStream.h>
#include <logic/context/LogicContext.h>
#include <p2p/modules/network/protocol/logic/sources/ConnectionSource.h>
#include <Poco/Net/NetException.h>
#include <logic/LogicExceptions.h>
#include "ClientConnection.h"


bsc::ClientConnection::ClientConnection(const Poco::Net::SocketAddress& a, const Context::Ptr& context)
        : bsc::Connection(
        context),
          socket(a) {

    auto lm = getConnectionContext()->get<LogicContext>();

    LOGGER("adding new connection, triggering connection established event")

    lm->getLogicManager().getSource<bsc::ConnectionSource>()->connectionEstablished(this);


}

void bsc::ClientConnection::startReceivingImpl() {
    bsc::Connection::startReceiving(socket);
    changeState(ConnectionState::CONNECTED);

}

void bsc::ClientConnection::startSendingImpl() {
    bsc::Connection::startSending(socket);

}

bsc::ClientConnection::~ClientConnection() {

    //@todo this socket is closed twice when client connection is destroyed. do something about it.
    try {
//        socket.shutdown();
        ClientConnection::shutdown();
    } catch (const Poco::Net::NetException& e) {
        LOGGER("net exception")
        LOGGER(e.displayText())
        auto nested = e.nested();
        while (nested != nullptr) {
            LOGGER(nested->displayText())
            nested = nested->nested();
        }
    }

}

void bsc::ClientConnection::shutdown() {
    bsc::Connection::shutdown();
    try {
        socket.shutdown();
    } catch (const Poco::Net::NetException& e) {
        LOGGER("net exception")
        e.displayText();
        auto nested = e.nested();
        while (nested != nullptr) {
            nested->displayText();
            nested = nested->nested();
        }
    }
    changeState(ConnectionState::DISCONNECTED);
    auto lc = getConnectionContext()->get<bsc::LogicContext>();
    auto& logicManager = lc->getLogicManager();
    auto connectionSourcePtr = logicManager.getSource<bsc::ConnectionSource>();
    connectionSourcePtr->connectionClosedClient(this);

}

Poco::Net::StreamSocket& bsc::ClientConnection::getSocket() {
    return socket;
}

bsc::ClientConnection::ClientConnection(const std::string& a, const Context::Ptr& ptr) : ClientConnection(
        Poco::Net::SocketAddress(a),
        ptr) {

}
