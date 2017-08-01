//
// Created by stilgar on 31.07.17.
//

#include <thread>
#include <iostream>
#include "ServerConnection.h"
#include <Poco/Net/SocketStream.h>

using namespace std::chrono_literals;

void ServerConnection::run() {

    auto streamSocket = socket();
    Poco::Net::SocketStream ss(streamSocket);
    cereal::BinaryInputArchive ia(ss);
    while (true) {
        // std::cout << "new loop" << std::endl;
        // if (streamSocket.available() > 0) {
        std::cout << "connection" << std::endl;


        std::shared_ptr<NetworkPacket> np;
        //NetworkPacket np;
        ia >> np;
        serverLogic->processPacket(np);


    }

    std::cout << "CLOSING CONNECTION";

}

ServerConnection::ServerConnection(const Poco::Net::StreamSocket &socket, Node &serverNode,
                                   const std::shared_ptr<IServerLogic> &serverLogic) : TCPServerConnection(socket),
                                                                                       serverNode(serverNode),
                                                                                       serverLogic(serverLogic) {}


ServerConnectionFactory::ServerConnectionFactory(Node &serverNode, const std::shared_ptr<IServerLogic> &serverLogic)
        : serverNode(serverNode), serverLogic(serverLogic) {}

Poco::Net::TCPServerConnection *ServerConnectionFactory::createConnection(const Poco::Net::StreamSocket &socket) {
    return new ServerConnection(socket, serverNode, serverLogic);
}
