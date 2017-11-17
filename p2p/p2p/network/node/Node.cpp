//
// Created by stilgar on 31.07.17.
//

#include <Poco/Net/SocketStream.h>
#include "Node.h"
#include "p2p/network/node/protocol/packet/NodeInfoRequest.h"
#include "p2p/network/node/protocol/packet/NodeInfoResponse.h"
#include <Poco/Net/NetException.h>
#include <p2p/network/protocol/connection/ServerConnection.h>
#include <p2p/network/protocol/connection/ClientConnection.h>
#include <p2p/network/protocol/context/NodeContext.h>
#include <p2p/network/protocol/context/LogicContext.h>
#include <p2p/configuration/ConfigurationManager.h>

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

    if (server != nullptr) {
        server->stop();
    }
    stopAcceptedConnections();

}

bool Node::connectTo(const NodeInfo &nodeInfo) {

    for (auto i : nodeInfo.getKnownAddresses()) {

        //try to connect
        if (connectTo(i)) {

            return true;
        }
    }

    return false;
}

Node::~Node() {
    std::lock_guard<std::mutex> g(acceptedConnectionsMutex);
    stop();


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

    initialize();
    //@todo this should be somewhere else
    ///@todo move to the ProtocolModule
    protocol->setupLogic(logicManager);
    logicManager.start();
    listen();
    startModules();


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

//    for (auto &i : activeClientConnections) {
//        //@todo implement
//        //test if connection is still alive
//        //process all messages from receive queue
//    }

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

    ///@todo debug remove
    ConfigurationManager manager;
    manager.save("Node", config);

    thisNodeInfo.addKnownAddress("127.0.0.1:" + std::to_string(getConfiguration()->getPort()));


}

std::shared_ptr<NetworkInfo> &Node::getNetworkInfo() {
    return networkInfo;
}

bool Node::isConnectedTo(const NodeInfo &nodeInfo) {
    bool ret = false;
    for (auto &&it : activeClientConnections) {
        if (it->nodeId) {
            ret |= nodeInfo.getNodeId() == (*it->nodeId);
        }
    }
    return ret;
}

void Node::updateNodeConnectionInfo() {
    //this is meant to be run from a thread
    for (auto &&item : activeClientConnections) {
        auto packet = NodeInfoRequest::getNew();
        NodeInfoResponse::Ptr response = protocol->sendExpect(item->connection.get(), packet);
        auto &ni = response->getNodeInfo();
        ni.printAll();
        auto nid = ni.getNodeId();
        LOGGER(ni.getNetworkId());
        const auto &val = response->getNodeInfo().getNodeId();
        //(*(*item).nodeId) = val;
        item->nodeId = val;
    }


}

void Node::purgeDuplicateConnections() {
    for (auto &&item : activeClientConnections) {
        if (item->nodeId) {
            //find all connections to the same id
            activeClientConnections.remove_if([&](NodeConnectionInfoPtr it) -> bool {
                if (it->nodeId) {
                    return *it->nodeId == *item->nodeId && it != item;
                } else {
                    return false;
                }
            });
            //activeClientConnections.erase(duplicate);
        }
    }
}

void Node::printConnections() {
    std::cout << "[" << this->thisNodeInfo.getNodeId() << "]:" << std::to_string(acceptedConnections.size())
              << std::endl;
    for (auto &&item : activeClientConnections) {
        if (!item->nodeId) {
            std::cout << "Connection: NO ID" << std::endl;
        } else {
            std::cout << "Connection: " << *item->nodeId << std::endl;
        }
    }
}

void Node::removeAcceptedConnection(IServerConnection *c) {
    // LOGGER("REMOVE REMOVE REMOVE");
    std::lock_guard<std::mutex> g(acceptedConnectionsMutex);
    acceptedConnections.remove(c);
}

void Node::addAcceptedConnection(IServerConnection *c) {
    std::lock_guard<std::mutex> g(acceptedConnectionsMutex);
    acceptedConnections.push_back(c);
}

void Node::stopAcceptedConnections() {
    for (auto &&it : acceptedConnections) {
        it->stop();
    }
    acceptedConnections.remove_if([](auto) { return true; });
}

void Node::purgeInactiveConnections() {
    activeClientConnections.remove_if([&](NodeConnectionInfoPtr it) -> bool {
        return it->connection == nullptr || !it->connection->isActive();
    });

}

void Node::startModules() {
    forEachModule<void>(&NodeModule::start);
}

void Node::initialize() {
    //initialize node modules
//    std::list<INodeModulePtr> modulesList;
//    modules.forEach(
//            [&](INodeModulePtr ptr) {
//                if (ptr != nullptr) {
//                    //ptr->initialize();
//                    //ptr->setupLogic(logicManager);
//                    modulesList.push_back(ptr);
//                };
//            });
//    auto sortedList = DependencyManager::dependencySort(modulesList);
//
//    for (auto &&item : sortedList) {
//        item->initialize();
//        item->setupLogic(logicManager);
//    }

    //this slightly changed the order of execution - instead of being initialized and setupLogic module by module,
    //now all modules are initialized and then all modules are setupLogiced
    //hope it doesn't break anything
    forEachModule(&INodeModule::initialize);
    forEachModule<bool, ILogicModule, LogicManager &>(&ILogicModule::setupLogic, logicManager);

}

void Node::stopModules() {
    forEachModule(&NodeModule::stop);

}

void Node::joinModules() {
    forEachModule(&NodeModule::join);

}

INodeModulePtr Node::getModuleByDependencyId(DependencyManager::TypeIdType id) {
    INodeModulePtr result = nullptr;
    modules.forEach([&](auto i) {
        if (i->getDependencyId() == id) {
            result = i;
            return;
        }
    });
    return result;
}

