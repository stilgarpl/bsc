//
// Created by stilgar on 01.08.17.
//

#include <Poco/Net/SocketStream.h>
#include "ClientConnection.h"

ClientConnection::ClientConnection(std::shared_ptr<Poco::Net::StreamSocket> socket) : socket(socket) {}

void ClientConnection::send(std::shared_ptr<NetworkPacket> np) {

    Poco::Net::SocketStream stream(*socket);
    cereal::BinaryOutputArchive oa(stream);
    oa << np;


}

std::shared_ptr<NetworkPacket> ClientConnection::receive() {
    std::shared_ptr<NetworkPacket> np;
    Poco::Net::SocketStream stream(*socket);
    cereal::BinaryInputArchive ia(stream);
    ia >> np;
}
