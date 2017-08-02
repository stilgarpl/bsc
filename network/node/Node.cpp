//
// Created by stilgar on 31.07.17.
//

#include <Poco/Net/SocketStream.h>
#include "Node.h"
#include "../protocol/ServerConnection.h"
#include "../protocol/ServerLogic.h"

using namespace Poco::Net;

void Node::listen() {
    //SocketAddress address("127.0.0.1:6777");
    if (serverSocket == nullptr) {
        //@TODO numer portu dac z configuracji
        ////@todo sprawdzanie bledow z bindowania socketa
        serverSocket = std::make_shared<ServerSocket>(6777);
    }
    server = std::make_shared<TCPServer>(new ServerConnectionFactory(*this, std::make_shared<ServerLogic>()),
                                         *serverSocket);
    server->start();
}


void Node::stopListening() {
    server->stop();

}

void Node::connectTo(const NodeInfo &nodeInfo) {

}

Node::~Node() {


}

ClientConnection Node::connectTo(const SocketAddress &address) {

    std::shared_ptr<Poco::Net::StreamSocket> socket = std::make_shared<Poco::Net::StreamSocket>(address);
    //@todo check for problems and handle them
    return ClientConnection(socket);
//    Poco::Net::SocketStream str(socket);
//
//    std::shared_ptr<NetworkPacket> np = std::make_shared<NetworkPacket>();
//   // NetworkPacket np;
//    std::cout << "connectTo" << std::endl;
//    np->print();
//    //np.print();
//    cereal::BinaryOutputArchive oa(str);
//
//    oa << np;
}

void Node::start() {

    listen();


}

void Node::stop() {

    stopListening();
}
