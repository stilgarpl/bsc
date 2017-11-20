//
// Created by stilgar on 07.11.17.
//


#include <p2p/network/node/protocol/logic/sources/NodeSource.h>
#include <p2p/network/node/protocol/logic/sources/NetworkSource.h>
#include <p2p/network/protocol/logic/sources/ConnectionSource.h>
#include <p2p/network/protocol/logic/sources/AuthSource.h>
#include <p2p/network/node/protocol/logic/actions/NodeActions.h>
#include <p2p/network/protocol/logic/actions/ProtocolActions.h>
#include <p2p/network/node/protocol/logic/actions/NetworkActions.h>
#include <p2p/network/node/protocol/packet/NodeInfoResponse.h>
#include <p2p/network/node/protocol/packet/NodeInfoRequest.h>
#include <p2p/network/protocol/connection/ClientConnection.h>
#include <Poco/Net/NetException.h>
#include "NodeNetworkModule.h"
#include "BasicModule.h"
#include <Poco/Net/SocketStream.h>
#include <p2p/network/protocol/connection/ServerConnection.h>


using namespace Poco::Net;

NodeNetworkModule::NodeNetworkModule(INode &node) : NodeModule(node) {
    setRequired<BasicModule>();
}

void NodeNetworkModule::setupActions(LogicManager &logicManager) {
    logicManager.setAction<ConnectionEvent>("reqNoI", NodeActions::sendNodeInfoRequest);
    logicManager.setAction<ConnectionEvent>("reqNeI", NodeActions::sendNetworkInfoRequest);
    logicManager.setAction<NodeInfoEvent>("upNoI", NodeActions::updateNodeInfo);
    logicManager.setAction<NetworkInfoEvent>("upNeI", NetworkActions::updateNetworkInfo);
    logicManager.setAction<NodeInfoEvent>("addKnownNode", NodeActions::addKnownNode);
    logicManager.setAction<Tick>("trigNodeUp", NodeActions::triggerUpdateNode);
    logicManager.setAction<NodeInfoEvent>("nodeDiscovered", NodeActions::newNodeDiscovered);


    logicManager.setAction<ConnectionEvent>("connDebug", [](const ConnectionEvent &event) {
        // std::clog << "Debug: connection event!" << std::endl;
    });
///@todo reenable
//    logicManager.setAction<PacketEvent>(PacketEventId::PACKET_RECEIVED,
//                                        [&transmissionControl](
//                                                const PacketEvent &packetEvent) {
//                                            return transmissionControl.onPacketReceived(packetEvent);
//                                        });
//    logicManager.setAction<PacketEvent>(PacketEventId::PACKET_SENT,
//                                        [&transmissionControl](const PacketEvent &packetEvent) {
//                                            return transmissionControl.onPacketSent(packetEvent);
//                                        });
//
//    logicManager.setAction<Tick>("TransTick", [&transmissionControl](const Tick &tick) {
//        return transmissionControl.work(tick);
//    });


    logicManager.setAction<ConnectionEvent>("onConnect", ProtocolActions::onNewConnection);


}

bool NodeNetworkModule::assignActions(LogicManager &logicManager) {
    if (logicManager.assignAction<ConnectionEvent>("connDebug")) {
        std::clog << "Debug: ConEv assignment!" << std::endl;

    }


//    if (logicManager.assignAction<ConnectionEvent>("onConnect")) {
//        std::clog << "Debug: onConn assignment!" << std::endl;
//
//    }

    if (logicManager.assignAction<ConnectionEvent>(ConnectionEvent::IdType::CONNECTION_ESTABLISHED, "reqNoI")) {
        std::clog << "Debug: reqNoI assignment!" << std::endl;

    }

    if (logicManager.assignAction<ConnectionEvent>(ConnectionEvent::IdType::CONNECTION_ESTABLISHED, "reqNeI")) {
        std::clog << "Debug: reqNeI assignment!" << std::endl;

    }

    if (logicManager.assignAction<NodeInfoEvent>(NodeInfoEvent::IdType::NODE_INFO_RECEIVED, "upNoI")) {
        std::clog << "Debug: upNoI assignment!" << std::endl;

    }

    if (logicManager.assignAction<NetworkInfoEvent>(NetworkInfoEvent::IdType::NETWORK_INFO_RECEIVED, "upNeI")) {
        std::clog << "Debug: upNoI assignment!" << std::endl;

    }

    ///@todo upNoI powinno cos takiego robic, addKnownNode powinien byc wywolany tylko w przypadku
    if (logicManager.assignAction<NodeInfoEvent>(NodeInfoEvent::IdType::NODE_INFO_RECEIVED, "addKnownNode")) {
        std::clog << "Debug: addKnownNode assignment!" << std::endl;

    }

    if (logicManager.assignAction<NodeInfoEvent>(NodeInfoEvent::IdType::NEW_NODE_DISCOVERED, "nodeDiscovered")) {
        std::clog << "Debug: addKnownNode assignment!" << std::endl;

    }

    ///@todo real value
    return true;
}

bool NodeNetworkModule::setupSources(LogicManager &logicManager) {
    logicManager.addSource<AuthSource>();

    logicManager.addSource<ConnectionSource>();
    logicManager.addSource<NetworkSource>();
    logicManager.addSource<NodeSource>();
    return true;
}

bool NodeNetworkModule::setupLogic(LogicManager &logicManager) {

    bool ret = ILogicModule::setupLogic(logicManager);
    ret &= protocol->setupLogic(logicManager);
    return ret;
}


void NodeNetworkModule::updateNodeConnectionInfo() {
    std::lock_guard<std::mutex> g(activeConnectionsMutex);
    std::lock_guard<std::mutex> g1(node.getLock());
    // std::thread([&]() {
    //    //this is meant to be run from a thread
    NODECONTEXTLOGGER("update node ")
        for (auto &&item : activeClientConnections) {
            NODECONTEXTLOGGER("update connection " + (item->nodeId ? *item->nodeId : " ?? "));
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

    //}).detach();
}

void NodeNetworkModule::purgeDuplicateConnections() {
    std::lock_guard<std::mutex> g(activeConnectionsMutex);
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

void NodeNetworkModule::printConnections() {
    std::lock_guard<std::mutex> g(activeConnectionsMutex);
    std::cout << "[" << node.getNodeInfo().getNodeId() << "]:" << std::to_string(acceptedConnections.size())
              << std::endl;
    for (auto &&item : activeClientConnections) {
        if (!item->nodeId) {
            std::cout << "Connection: NO ID" << std::endl;
        } else {
            std::cout << "Connection: " << *item->nodeId << std::endl;
        }
    }
}

void NodeNetworkModule::purgeInactiveConnections() {
    std::lock_guard<std::mutex> g(activeConnectionsMutex);
    activeClientConnections.remove_if([&](NodeConnectionInfoPtr it) -> bool {
        return it->connection == nullptr || !it->connection->isActive();
    });

}

void NodeNetworkModule::addActiveClientConnection(std::shared_ptr<Connection> c) {
    std::lock_guard<std::mutex> g(activeConnectionsMutex);
    NodeConnectionInfoPtr info = std::make_shared<NodeConnectionInfo>();
    info->connection = c;
    info->nodeId = std::experimental::nullopt;

    activeClientConnections.push_back(info);

}

void NodeNetworkModule::removeActiveClientConnection(NodeConnectionInfoPtr c) {
    std::lock_guard<std::mutex> g(activeConnectionsMutex);
    //  auto el = std::find(activeClientConnections.begin(),activeClientConnections.end(),c);
    activeClientConnections.remove(c);

}

void NodeNetworkModule::removeAcceptedConnection(IServerConnection *c) {
    // LOGGER("REMOVE REMOVE REMOVE");
    std::lock_guard<std::mutex> g(acceptedConnectionsMutex);
    acceptedConnections.remove(c);
}

void NodeNetworkModule::addAcceptedConnection(IServerConnection *c) {
    std::lock_guard<std::mutex> g(acceptedConnectionsMutex);
    acceptedConnections.push_back(c);
}

void NodeNetworkModule::stopAcceptedConnections() {
    std::lock_guard<std::mutex> g(acceptedConnectionsMutex);
    for (auto &&it : acceptedConnections) {
        it->stop();
    }
    acceptedConnections.remove_if([](auto) { return true; });
}


void NodeNetworkModule::listen() {
    //SocketAddress address("127.0.0.1:6777");
    if (serverSocket == nullptr) {
        unsigned short port = 6777;
        //@TODO numer portu dac z configuracji
        //@todo FIX THAT CAST
        ////@todo sprawdzanie bledow z bindowania socketa
        if (((Node *) &node)->getConfiguration() != nullptr) {
            port = ((Node *) &node)->getConfiguration()->getPort();
        }
        serverSocket = std::make_shared<ServerSocket>(port);
    }

    server = std::make_shared<TCPServer>(
            new ServerConnectionFactory(static_cast<Node &>(this->node), this->node.getContext()),
            *serverSocket);
    server->start();

}


void NodeNetworkModule::stopListening() {

    if (server != nullptr) {
        server->stop();
    }
    stopAcceptedConnections();

}

bool NodeNetworkModule::connectTo(const NodeInfo &nodeInfo) {

    for (auto i : nodeInfo.getKnownAddresses()) {

        //try to connect
        if (connectTo(i)) {

            return true;
        }
    }

    return false;
}

void NodeNetworkModule::onStop() {

    //  stopListening();
}

bool NodeNetworkModule::connectTo(const SocketAddress &address) {

    //std::shared_ptr<Poco::Net::StreamSocket> socket = std::make_shared<Poco::Net::StreamSocket>(address);
    //@todo check for problems and handle them
    try {
        std::shared_ptr<ClientConnection> connection = std::make_shared<ClientConnection>(address,
                                                                                          std::ref(node.getContext()));
        addActiveClientConnection(connection);
        connection->startSending();
        connection->startReceiving();
        return true;
    } catch (Poco::Net::ConnectionRefusedException) {
        return false;
    }
}

void NodeNetworkModule::onStart() {
    Runnable::onStart();
    listen();
}


bool NodeNetworkModule::connectTo(const std::string &a) {
    Poco::Net::SocketAddress address(a);
    return connectTo(address);
}


bool NodeNetworkModule::isConnectedTo(const NodeInfo &nodeInfo) {
    bool ret = false;
    for (auto &&it : activeClientConnections) {
        if (it->nodeId) {
            ret |= nodeInfo.getNodeId() == (*it->nodeId);
        }
    }
    return ret;
}
