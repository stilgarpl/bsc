//
// Created by stilgar on 31.07.17.
//

#include <Poco/Net/SocketStream.h>
#include "Node.h"
#include "protocol/packet/NodeInfoRequest.h"
#include "protocol/packet/NodeInfoResponse.h"
#include <Poco/Net/NetException.h>
#include <network/protocol/connection/ServerConnection.h>
#include <network/protocol/connection/ClientConnection.h>
#include <network/protocol/context/NodeContext.h>
#include <network/protocol/context/LogicContext.h>

using namespace Poco::Net;

void Node::listen() {
    //SocketAddress address("127.0.0.1:6777");
    if (serverSocket == nullptr) {
        unsigned short port = 6777;
        //@TODO numer portu dac z configuracji
        ////@todo sprawdzanie bledow z bindowania socketa
        if (configuration != nullptr) {
            port = configuration->getPort();
        }
        serverSocket = std::make_shared<ServerSocket>(port);
    }

    server = std::make_shared<TCPServer>(new ServerConnectionFactory(*this, this->nodeContext),
                                         *serverSocket);
    server->start();

}


void Node::stopListening() {
    server->stop();

}

bool Node::connectTo(const NodeInfo &nodeInfo) {

    for (auto i : nodeInfo.getKnownAddresses()) {

        //try to connect
        if (connectTo(i)) {

            break;
        }
    }

}

Node::~Node() {


}

bool Node::connectTo(const SocketAddress &address) {

    //std::shared_ptr<Poco::Net::StreamSocket> socket = std::make_shared<Poco::Net::StreamSocket>(address);
    //@todo check for problems and handle them
    try {
        std::shared_ptr<ClientConnection> connection = std::make_shared<ClientConnection>(address,
                                                                                          std::ref(nodeContext));
        addActiveClientConnection(connection);
        connection->startSending();
        connection->startReceiving();
        return true;
    } catch (Poco::Net::ConnectionRefusedException) {
        return false;
    }
}

void Node::start() {

    //@todo this should be somewhere else
    protocol->setupLogic(logicManager);
    logicManager.start();
    listen();


}

void Node::stop() {

    logicManager.stop();
    stopListening();
}

void Node::addActiveClientConnection(std::shared_ptr<Connection> c) {
    NodeConnectionInfoPtr info = std::make_shared<NodeConnectionInfo>();
    info->connection = c;
    info->nodeId = std::experimental::nullopt;

    activeClientConnections.push_back(info);

}

void Node::removeActiveClientConnection(NodeConnectionInfoPtr c) {
    //  auto el = std::find(activeClientConnections.begin(),activeClientConnections.end(),c);
    activeClientConnections.remove(c);

}

bool Node::connectTo(const std::string &a) {
    Poco::Net::SocketAddress address(a);
    return connectTo(address);
}

void Node::work() {

    for (auto &i : activeClientConnections) {
        //@todo implement
        //test if connection is still alive
        //process all messages from receive queue
    }

}

const std::shared_ptr<Node::Config> &Node::getConfiguration() const {
    return configuration;
}

void Node::setConfiguration(const std::shared_ptr<Node::Config> &configuration) {
    Node::configuration = configuration;
}

Node::Node() {

    nodeContext.set<NodeContext, Node &, NodeInfo &>(*this, this->thisNodeInfo);
    nodeContext.set<LogicContext, LogicManager &>(logicManager);

    logicManager.setContexts(nodeContext);



}

Node::Node(int port) : Node() {
    std::shared_ptr<Node::Config> config = std::make_shared<Node::Config>();
    config->setPort(port);
    setConfiguration(config);
    thisNodeInfo.addKnownAddress("127.0.0.1:" + std::to_string(getConfiguration()->getPort()));


}

std::shared_ptr<NetworkInfo> &Node::getNetworkInfo() {
    return networkInfo;
}

bool Node::isConnectedTo(const NodeInfo &nodeInfo) {
    for (auto &&it : activeClientConnections) {

    }
}

void Node::updateNodeConnectionInfo() {
    //this is meant to be run from a thread
    for (auto &&item : activeClientConnections) {
        BasePacketPtr packet = std::make_shared<NodeInfoRequest>();
        auto future = protocol->send(item->connection.get(), packet);
        future.wait();
        NetworkPacketPointer<NodeInfoResponse> response = std::dynamic_pointer_cast<NodeInfoResponse>(future.get());
        auto val = response->getNodeInfo().getNodeId();
        (*(*item).nodeId) = val;
        //item->nodeId = val;
    }


}

