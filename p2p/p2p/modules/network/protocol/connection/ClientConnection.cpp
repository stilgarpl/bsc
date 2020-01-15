#include <utility>

//
// Created by stilgar on 01.08.17.
//

#include <Poco/Net/SocketStream.h>
#include <logic/context/LogicContext.h>
#include <p2p/modules/network/protocol/logic/sources/ConnectionSource.h>
#include <Poco/Net/NetException.h>
#include <logic/LogicExceptions.h>
#include "ClientConnection.h"


ClientConnection::ClientConnection(const Poco::Net::SocketAddress& a, const bsc::Context::Ptr& context) : Connection(
        context),
                                                                                                          socket(a) {

    auto& lm = getConnectionContext()->get<LogicContext>();

    LOGGER("adding new connection, triggering connection established event")

    lm.getLogicManager().getSource<ConnectionSource>()->connectionEstablished(this);


}

void ClientConnection::startReceivingImpl() {
    Connection::startReceiving(socket);
    changeState(ConnectionState::CONNECTED);

}

void ClientConnection::startSendingImpl() {
    Connection::startSending(socket);

}

ClientConnection::~ClientConnection() {

    //@todo this socket is closed twice when client connection is destroyed. do something about it.
    try {
//        socket.shutdown();
        ClientConnection::shutdown();
    } catch (const Poco::Net::NetException &e) {
        LOGGER("net exception")
        LOGGER(e.displayText());
        auto nested = e.nested();
        while (nested != nullptr) {
            LOGGER(nested->displayText());
            nested = nested->nested();
        }
    }

}

void ClientConnection::shutdown() {
    Connection::shutdown();
    try {
        socket.shutdown();
    } catch (const Poco::Net::NetException &e) {
        LOGGER("net exception")
        e.displayText();
        auto nested = e.nested();
        while (nested != nullptr) {
            nested->displayText();
            nested = nested->nested();
        }
    }
    changeState(ConnectionState::DISCONNECTED);
    auto& lc = getConnectionContext()->get<LogicContext>();
    auto &logicManager = lc.getLogicManager();
    auto connectionSourcePtr = logicManager.getSource<ConnectionSource>();
    connectionSourcePtr->connectionClosedClient(this);

}

Poco::Net::StreamSocket &ClientConnection::getSocket() {
    return socket;
}

ClientConnection::ClientConnection(const std::string& a, bsc::Context::Ptr ptr) : ClientConnection(
        Poco::Net::SocketAddress(a),
        std::move(ptr)) {

}
