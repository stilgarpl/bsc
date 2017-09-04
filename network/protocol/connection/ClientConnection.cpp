//
// Created by stilgar on 01.08.17.
//

#include <Poco/Net/SocketStream.h>
#include "ClientConnection.h"
#include "../logic/sources/ConnectionSource.h"
#include "../../../logic/LogicManager.h"
#include "../context/LogicContext.h"


ClientConnection::ClientConnection(const Poco::Net::SocketAddress &a, Context &context) : socket(a),
                                                                                          Connection(context) {

    auto lm = getConnectionContext().get<LogicContext>();
    if (lm != nullptr) {
        std::clog << __func__ << " adding new connection!!!!!!!!!!!!!1 " << std::endl;

        lm->getLogicManager().getSource<ConnectionSource>()->newConnection(this);
    } else {
        std::clog << __func__ << " no logic manager in context!" << std::endl;
    }

}

void ClientConnection::startReceiving() {
    Connection::startReceiving(socket);

}

void ClientConnection::startSending() {
    Connection::startSending(socket);

}
