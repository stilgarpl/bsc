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
#include <p2p/modules/configuration/ConfigurationModule.h>
#include <p2p/network/protocol/context/NodeContext.h>




using namespace Poco::Net;

NodeNetworkModule::NodeNetworkModule(INode &node) : NodeModuleConfigDependent(node) {
//    setRequired<BasicModule>();
}

void NodeNetworkModule::setupActions(ILogicModule::SetupActionHelper &actionHelper) {
    actionHelper.setAction<ConnectionEvent>("reqNoI", NodeActions::sendNodeInfoRequest);
    actionHelper.setAction<ConnectionEvent>("reqNeI", NodeActions::sendNetworkInfoRequest);
    actionHelper.setAction<NodeInfoEvent>("upNoI", NodeActions::updateNodeInfo);
    actionHelper.setAction<NetworkInfoEvent>("upNeI", NetworkActions::updateNetworkInfo);
    actionHelper.setAction<NodeInfoEvent>("addKnownNode", NodeActions::addKnownNode);
    actionHelper.setAction<Tick>("trigNodeUp", NodeActions::triggerUpdateNode);
    actionHelper.setAction<NodeInfoEvent>("nodeDiscovered", NodeActions::newNodeDiscovered);


    actionHelper.setAction<ConnectionEvent>("connDebug", [](const ConnectionEvent &event) {
        // std::clog << "Debug: connection event!" << std::endl;
    });


    actionHelper.setAction<ConnectionEvent>(ConnectionEventId::CONNECTION_CLOSED_CLIENT,
                                            [&](const ConnectionEvent &event) {
                                                this->removeActiveClientConnection(event.getConnection());
                                            });

//    actionHelper.setAction<ConnectionEvent>(ConnectionEventId::CONNECTION_CLOSED_SERVER, [&](const ConnectionEvent &event) {
//        ///@todo do something about this dynamic cast
//        this->removeAcceptedConnection(dynamic_cast<IServerConnection *>(event.getConnection()));
//    });
///@todo reenable
//    actionHelper.setAction<PacketEvent>(PacketEventId::PACKET_RECEIVED,
//                                        [&transmissionControl](
//                                                const PacketEvent &packetEvent) {
//                                            return transmissionControl.onPacketReceived(packetEvent);
//                                        });
//    actionHelper.setAction<PacketEvent>(PacketEventId::PACKET_SENT,
//                                        [&transmissionControl](const PacketEvent &packetEvent) {
//                                            return transmissionControl.onPacketSent(packetEvent);
//                                        });
//
//    actionHelper.setAction<Tick>("TransTick", [&transmissionControl](const Tick &tick) {
//        return transmissionControl.work(tick);
//    });


    actionHelper.setAction<ConnectionEvent>("onConnect", ProtocolActions::onNewConnection);


}

bool NodeNetworkModule::assignActions(ILogicModule::AssignActionHelper &actionHelper) {
    if (actionHelper.assignAction<ConnectionEvent>("connDebug")) {
        std::clog << "Debug: ConEv assignment!" << std::endl;

    }

    if (actionHelper.assignAction<ConnectionEvent>(ConnectionEvent::IdType::CONNECTION_ESTABLISHED, "reqNoI")) {
        std::clog << "Debug: reqNoI assignment!" << std::endl;

    }

    if (actionHelper.assignAction<ConnectionEvent>(ConnectionEvent::IdType::CONNECTION_ESTABLISHED, "reqNeI")) {
        std::clog << "Debug: reqNeI assignment!" << std::endl;

    }

    if (actionHelper.assignAction<ConnectionEvent>(ConnectionEventId::CONNECTION_CLOSED_CLIENT,
                                                   ConnectionEventId::CONNECTION_CLOSED_CLIENT)) {
        std::clog << "Debug: CONNECTION_CLOSED assignment!" << std::endl;

    }


    if (actionHelper.assignAction<NodeInfoEvent>(NodeInfoEvent::IdType::NODE_INFO_RECEIVED, "upNoI")) {
        std::clog << "Debug: upNoI assignment!" << std::endl;

    }

    if (actionHelper.assignAction<NetworkInfoEvent>(NetworkInfoEvent::IdType::NETWORK_INFO_RECEIVED, "upNeI")) {
        std::clog << "Debug: upNoI assignment!" << std::endl;

    }

    ///@todo upNoI powinno cos takiego robic, addKnownNode powinien byc wywolany tylko w przypadku
    if (actionHelper.assignAction<NodeInfoEvent>(NodeInfoEvent::IdType::NODE_INFO_RECEIVED, "addKnownNode")) {
        std::clog << "Debug: addKnownNode assignment!" << std::endl;

    }

    if (actionHelper.assignAction<NodeInfoEvent>(NodeInfoEvent::IdType::NEW_NODE_DISCOVERED, "nodeDiscovered")) {
        std::clog << "Debug: addKnownNode assignment!" << std::endl;

    }




    //high level logic:

//    when(event<ConnectionEvent>().withId(ConnectionEvent::IdType::CONNECTION_ESTABLISHED)).


    ///@todo real value
    return true;
}

bool NodeNetworkModule::setupSources(ILogicModule::SetupSourceHelper &sourceHelper) {
    sourceHelper.requireSource<AuthSource>();

    sourceHelper.requireSource<ConnectionSource>();
    sourceHelper.requireSource<NetworkSource>();
    sourceHelper.requireSource<NodeSource>();
    return true;
}

bool NodeNetworkModule::setupLogic() {

    bool ret = ILogicModule::setupLogic();
    ret &= protocol->setupLogic();
    return ret;
}


void NodeNetworkModule::updateNodeConnectionInfo() {
    ///@todo this is wrong, it blocks connections until its done.
    std::lock_guard<std::mutex> g(activeConnectionsMutex);
    //   std::lock_guard<std::mutex> g1(node.getLock());
    // std::thread([&]() {
    //    //this is meant to be run from a thread
    NODECONTEXTLOGGER("update node ")
        for (auto &&item : activeClientConnections) {
            NODECONTEXTLOGGER("update connection " + (item->nodeId ? *item->nodeId : " ?? "));
            auto packet = NodeInfoRequest::getNew();
            NodeInfoResponse::Ptr response = protocol->sendExpect(item->connection.get(), packet);
            if (response != nullptr) {
                auto &ni = response->getNodeInfo();
                ni.printAll();
                auto nid = ni.getNodeId();
                LOGGER(ni.getNetworkId());
                const auto &val = response->getNodeInfo().getNodeId();
                //(*(*item).nodeId) = val;
                item->nodeId = val;
            } else {
                //response not received, connection probably is down
                ///@todo decide what do to, disconnecting for now
                item->connection->shutdown();
            }
        }

    //}).detach();
}

void NodeNetworkModule::purgeDuplicateConnections() {
//    std::lock_guard<std::mutex> g(activeConnectionsMutex);
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
    info->nodeId = std::nullopt;

    activeClientConnections.push_back(info);

}

void NodeNetworkModule::removeActiveClientConnection(NodeConnectionInfoPtr c) {
    std::lock_guard<std::mutex> g(activeConnectionsMutex);
    //  auto el = std::find(activeClientConnections.begin(),activeClientConnections.end(),c);
    activeClientConnections.remove(c);

}

void NodeNetworkModule::removeActiveClientConnection(Connection *c) {
    std::lock_guard<std::mutex> g(activeConnectionsMutex);
    //  auto el = std::find(activeClientConnections.begin(),activeClientConnections.end(),c);
    activeClientConnections.erase(std::remove_if(activeClientConnections.begin(), activeClientConnections.end(),
                                                 [&](NodeConnectionInfoPtr &i) -> bool {
                                                     return (i->connection.get() == c);
                                                 }), activeClientConnections.end());

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
    ///@todo this segfaults, why bother stopping those connections? stopping the poco server should stop them anyway.
    for (auto &&it : acceptedConnections) {
        if (it != nullptr)
            it->stop();
    }
    acceptedConnections.remove_if([](auto) { return true; });
}


void NodeNetworkModule::listen() {
    //SocketAddress address("127.0.0.1:6777");
    if (serverSocket == nullptr) {
//        unsigned short port = 6777;
        //@TODO numer portu dac z configuracji
        //@todo FIX THAT CAST
        ////@todo sprawdzanie bledow z bindowania socketa
        ///@todo ConfigurationModule
//        if (((Node *) &node)->getConfiguration() != nullptr) {
//            port = ((Node *) &node)->getConfiguration()->getPort();
//        }

        ///@todo configuration should be loaded on demand, not here
//        auto loadedConfig = node.getModule<ConfigurationModule>()->load<Config>("network");
//        if (loadedConfig != nullptr) {
//            configuration() = *loadedConfig;
//        }

//        ///@todo debug
//        node.getModule<ConfigurationModule>()->save("network", loadedConfig);
        serverSocket = std::make_shared<ServerSocket>(configuration().getPort());
        LOGGER(std::string("Opening port") + std::to_string(configuration().getPort()));
    }


    server = std::make_shared<TCPServer>(
            new ServerConnectionFactory(static_cast<Node &>(this->node), this->node.getContext()),
            *serverSocket);
    server->start();

}


void NodeNetworkModule::stopListening() {
    stopAcceptedConnections();
    if (server != nullptr) {
        server->stop();
    }


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

    stopListening();
    disconnectAll();
}

bool NodeNetworkModule::connectTo(const SocketAddress &address) {

    //std::shared_ptr<Poco::Net::StreamSocket> socket = std::make_shared<Poco::Net::StreamSocket>(address);
    //@todo check for problems and handle them
    try {
        std::shared_ptr<ClientConnection> connection = std::make_shared<ClientConnection>(address,
                                                                                          std::ref(node.getContext()));
        ///@todo maybe this should be a reacting to an CONNECTION_ESTABLISHED event?
        addActiveClientConnection(connection);
        connection->startSending();
        connection->startReceiving();
        return true;
    } catch (Poco::Net::ConnectionRefusedException) {
        ///@todo connection refused in connectionSource
        return false;
    } catch (Poco::InvalidArgumentException) {
        return false;
    }
}

void NodeNetworkModule::onStart() {
    Runnable::onStart();
    listen();
}


bool NodeNetworkModule::connectTo(const std::string &a) {
    try {
        Poco::Net::SocketAddress address(a);
        return connectTo(address);
    }
    catch (Poco::InvalidArgumentException) {
        ///@todo bad address from connection source
        return false;
    }

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

void NodeNetworkModule::run() {
    NodeModule::run();

    ///@todo find a bettter way to trigger onStop after run
    while (!isStopping()) {
        std::this_thread::sleep_for(400ms);
    }
}
//
//bool NodeNetworkModule::sendPacketToNode(const NodeIdType &nodeId, BasePacketPtr packet) {
//    ConnectionPtr conn = nullptr;
//    for (auto &&connection : activeClientConnections) {
//        if (connection->nodeId && connection->nodeId == nodeId) {
//            conn = connection->connection;
//        }
//    }
//    if (conn == nullptr) {
//        ///@todo connect to node
//    }
//
//    if (conn != nullptr) {
//        conn->send(packet);
//        LOGGER("sending packet to node " + nodeId)
//        return true;
//    } else {
//        LOGGER("unable to send packet to " + nodeId)
//        return false;
//
//    }
//}

bool NodeNetworkModule::connectToAddress(const std::string &add) {
    return connectTo(add);
}

void NodeNetworkModule::disconnect(const NodeIdType id) {
    {
        std::lock_guard<std::mutex> g(activeConnectionsMutex);
        for (auto &&item : activeClientConnections) {
            if (item->nodeId && item->nodeId == id && item->connection != nullptr) {
                item->connection->shutdown();
                // activeClientConnections.remove(item);
            }
        }
    }
    purgeInactiveConnections();
}

void NodeNetworkModule::disconnectAll() {
    std::lock_guard<std::mutex> g(activeConnectionsMutex);
    //activeClientConnections.remove_if([](auto i) { return true; });
    for (auto &&item : activeClientConnections) {
        if (item->connection != nullptr) {
            item->connection->shutdown();
        }

    }
    activeClientConnections.clear();

}

const std::unique_ptr<IProtocol> &NodeNetworkModule::getProtocol() const {
    return protocol;
}
