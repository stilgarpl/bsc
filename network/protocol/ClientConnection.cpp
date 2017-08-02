//
// Created by stilgar on 01.08.17.
//

#include <Poco/Net/SocketStream.h>
#include "ClientConnection.h"

ClientConnection::ClientConnection(std::shared_ptr<Poco::Net::StreamSocket> socket) : socket(socket) {}

