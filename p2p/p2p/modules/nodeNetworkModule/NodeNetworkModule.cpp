//
// Created by stilgar on 07.11.17.
//


#include <p2p/modules/nodeNetworkModule/protocol/logic/sources/NodeSource.h>
#include <p2p/modules/nodeNetworkModule/protocol/logic/sources/NetworkSource.h>
#include <p2p/modules/nodeNetworkModule/protocol/logic/sources/ConnectionSource.h>
#include <p2p/modules/nodeNetworkModule/protocol/logic/actions/NodeActions.h>
#include <p2p/modules/nodeNetworkModule/protocol/logic/actions/ProtocolActions.h>
#include <p2p/modules/nodeNetworkModule/protocol/logic/actions/NetworkActions.h>
#include <p2p/modules/nodeNetworkModule/protocol/connection/ClientConnection.h>
#include <Poco/Net/NetException.h>
#include "NodeNetworkModule.h"
#include "p2p/modules/basic/BasicModule.h"
#include <Poco/Net/SocketStream.h>
#include <p2p/modules/nodeNetworkModule/protocol/connection/ServerConnection.h>
#include <p2p/modules/configuration/ConfigurationModule.h>
#include <p2p/node/context/NodeContext.h>
#include <p2p/modules/nodeNetworkModule/protocol/packet/KeepAlivePacket.h>
#include <p2p/modules/nodeNetworkModule/protocol/packet/NodeInfoGroup.h>
#include <p2p/logic/state/LogicStateMachine.h>
#include <p2p/logic/evaluators/CommonEvaluators.h>
#include <p2p/logic/actions/CommonActions.h>
#include <p2p/logic/chain/ChainEvaluators.h>


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


//    actionHelper.setAction<ConnectionEvent>(ConnectionEventId::CONNECTION_CLOSED_CLIENT,
//                                            [&](const ConnectionEvent &event) {
//                                                this->removeActiveClientConnection(event.getConnection());
//                                            });

//    actionHelper.setAction<ConnectionEvent>(ConnectionEventId::CONNECTION_CLOSED_SERVER, [&](const ConnectionEvent &event) {
//        //@todo do something about this dynamic cast
//        this->removeAcceptedConnection(dynamic_cast<IServerConnection *>(event.getConnection()));
//    });
//@todo reenable
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

    //@todo upNoI powinno cos takiego robic, addKnownNode powinien byc wywolany tylko w przypadku
    if (actionHelper.assignAction<NodeInfoEvent>(NodeInfoEvent::IdType::NODE_INFO_RECEIVED, "addKnownNode")) {
        std::clog << "Debug: addKnownNode assignment!" << std::endl;

    }

    if (actionHelper.assignAction<NodeInfoEvent>(NodeInfoEvent::IdType::NEW_NODE_DISCOVERED, "nodeDiscovered")) {
        std::clog << "Debug: addKnownNode assignment!" << std::endl;

    }


    //register packet processors from submodules

    when(state<ILogicModule>(ModuleState::SUBMODULES_PREPARED).entered()).fireStateChangeReaction(
            [&](ILogicModule &module) {
                //@todo move this mechanism to NodeModule to auto collect all submodules from other modules.
                //  submodules probably have to be optional or sth.
                auto &networkSub = module.getSubModule<NodeNetworkModule>();
                networkSub.setupPacketProcessing(*this);
            });



    //high level logic:

//    when(event<ConnectionEvent>().withId(ConnectionEvent::IdType::CONNECTION_ESTABLISHED)).

    //send keepalives - I tried using it to help with the packet loss, but it didn't work.
    when(event<Tick>(120s)).fireNewAction(
            [&, this](auto e) { this->broadcastPacket(KeepAlivePacket::Request::getNew()); });

    when(event<ModuleEvent<NodeNetworkModule>>(ModuleState::READY)).fireNewAction(
            NetworkActions::loadNetworkInfo);

    when(event<ModuleEvent<NodeNetworkModule>>(ModuleState::SHUTDOWN)).fireNewAction(
            NetworkActions::saveNetworkInfo);

    when(event<ModuleEvent<NodeNetworkModule>>()).fireNewAction([](auto event) { LOGGER("module event NNM") });
//
//    auto list = {88, 77, 44, 101};
//    auto list2 = {99, 10};
//    auto stage1 = when(event < Tick > (1s)).newChain("chain_test").fireNewChainAction(testingMethod3s, "1");
////    stage1.
//    auto stage2 = stage1.fireNewGenericChainAction(testingMethod1s, [](auto tick) -> std::string {
//        static int rr = 0;
//        rr++;
//        return std::string("lala " + std::to_string(rr));
//    }).fireNewGenericChainAction(testingMethod1s, CommonEvaluators::unwrapEvent);
//
//    stage2.fireNewGenericChainAction(testingMethod1xs,
//                                     ChainEvaluators::genericChainResult(stage2));
//            stage2.fireNewGenericChainAction(CommonActions::foreachAction(testingMethod1i, list),
//                                      CommonEvaluators::foreachValue<int>());

//    when(event < SpecificPacketEvent<KeepAlivePacket::Response>>
//    (PacketEventId::PACKET_RECEIVED)).fireNewAction([](auto event) {
//        LOGGER("SPECIFIC EVENT: keepalive received!");
//    });
//        stage1.fireNewChainAction([](Tick tick){return IEvent<int>(51);}).fireNewChainAction([](IEvent<int> e, std::string w){LOGGER("lwlwl " +std::to_string(e.getEventId())); return e;}, "7");
//
//    stage1.fireNewChainAction(testingMethod3s, "2");
//    stage1.fireNewChainAction(testingMethod3s, "a").fireNewChainAction(testingMethod3s, "b").fireNewChainAction(
//            testingMethod3s, "w");
//    stage1.fireNewChainAction(testingMethod3s, "x");


    //@todo move to more appropriate method
//    Roles::defineRequiredRole<NodeInfoGroup::Request>("testRole");
//    Roles::defineRequiredRole<NodeInfoGroup::Request>("testRole2");


    LogicStateMachine<NodeNetworkModule, int> intStateMachine(*this);
//    when(event < LogicStateEvent<NodeNetworkModule, int>>
//    ()).fireModuleAction(&NodeNetworkModule::testingMethod2);
//    when(state<NodeNetworkModule, int>(2).entered()/*.left()*/).valid();
//    when(state<NodeNetworkModule, int>(4).entered()/*.left()*/).fireStateChangeReaction([](auto &netMod, auto a) {
//        LOGGER("net mode test.. " + std::to_string(a) + "  " + netMod.testingMethod())
//    });//.fireModuleAction(&NodeNetworkModule::testingMethod);
    when(event < LogicStateEvent<NodeNetworkModule, int>>
    ()).fireNewAction([](auto event) {
        NODECONTEXTLOGGER("logic state event " + std::to_string(event.getState()) +
                          (event.getMethod() == LogicStateMethod::ENTERED ? " ENTERED" : " LEFT"))
    });

//    Tick tick;
//    tick.getNow().time_since_epoch().
//    when(event < Tick > (1s)).constraint([](Tick &event) -> bool {
//        return std::chrono::duration_cast<std::chrono::seconds>(event.getNow().time_since_epoch()).count() % 7 == 0;
//    }).fireNewAction([](auto event) { LOGGER("every 5s!") });
//    intStateMachine.define(1).to(2).to(3).to(4);
//    auto def = intStateMachine.define(1);
//    def->(2)->(3);
//    intStateMachine.addState(1, 2, 3, 4, 5, 6);
//    intStateMachine.addLink(1, 2, 3, 4, 5, 6);
//    intStateMachine.setState(2);
//    intStateMachine.changeState(3);
//    intStateMachine.changeState(4);
//    intStateMachine.changeState(6);

    //@todo real value
    return true;
}

bool NodeNetworkModule::setupSources(ILogicModule::SetupSourceHelper &sourceHelper) {


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
    //@todo this is wrong, it blocks connections until its done.
    std::lock_guard<std::mutex> g(activeConnectionsMutex);
    //   std::lock_guard<std::mutex> g1(node.getLock());
    // std::thread([&]() {
    //    //this is meant to be run from a thread
//    NODECONTEXTLOGGER("update node ")
//        for (auto &&item : activeClientConnections) {
//            NODECONTEXTLOGGER("update connection " + (item->nodeId ? *item->nodeId : " ?? "));
//            auto packet = NodeInfoRequest::getNew();
//            NodeInfoResponse::Ptr response = protocol->sendExpect(item->connection.get(), packet);
//            if (response != nullptr) {
//                auto &ni = response->getNodeInfo();
//                ni.printAll();
//                auto nid = ni.getNodeId();
//                LOGGER(ni.getNetworkId());
//                const auto &val = response->getNodeInfo().getNodeId();
//                //(*(*item).nodeId) = val;
//                item->nodeId = val;
//            } else {
//                //response not received, connection probably is down
//                //@todo decide what do to, disconnecting for now
//                item->connection->shutdown();
//            }
//        }

    //}).detach();
}

void NodeNetworkModule::purgeDuplicateConnections() {
//    std::lock_guard<std::mutex> g(activeConnectionsMutex);
//    for (auto &&item : activeClientConnections) {
//        if (item->nodeId) {
//            //find all connections to the same id
//            activeClientConnections.remove_if([&](NodeConnectionInfoPtr it) -> bool {
//                if (it->nodeId) {
//                    return *it->nodeId == *item->nodeId && it != item;
//                } else {
//                    return false;
//                }
//            });
//            //activeClientConnections.erase(duplicate);
//        }
//    }
}

void NodeNetworkModule::printConnections() {
    std::lock_guard<std::mutex> g(activeConnectionsMutex);
    std::cout << "[" << node.getNodeInfo().getNodeId() << "]:" << std::to_string(acceptedConnections.size())
              << std::endl;
    for (auto &&item : remoteNodes) {
        std::string pre = "[ ]";
        if (item->isConnected()) {
            pre = "[X]";
        }
        if (!item->getNodeId()) {
            std::cout << pre << "Connection: NO ID" << std::endl;
        } else {
            std::cout << pre << "Connection: " << *item->getNodeId() << std::endl;
        }
    }
}

void NodeNetworkModule::purgeInactiveConnections() {
//    std::lock_guard<std::mutex> g(activeConnectionsMutex);
//    activeClientConnections.remove_if([&](NodeConnectionInfoPtr it) -> bool {
//        return it->connection == nullptr || !it->connection->isActive();
//    });

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
    //@todo this segfaults, why bother stopping those connections? stopping the poco server should stop them anyway.
    for (auto &&it : acceptedConnections) {
        if (it != nullptr)
            it->stop();
    }
    acceptedConnections.remove_if([](auto) { return true; });
}


void NodeNetworkModule::listen() {
    //SocketAddress address("127.0.0.1:6777");
    if (serverSocket == nullptr) {

//        //@todo debug
//        node.getModule<ConfigurationModule>()->save("network", loadedConfig);
        serverSocket = std::make_shared<ServerSocket>(configuration().getPort());
        LOGGER(std::string("Opening port") + std::to_string(configuration().getPort()));
    }


    server = std::make_shared<TCPServer>(
            new ServerConnectionFactory(dynamic_cast<Node &>(this->node), this->node.getContext()),
            *serverSocket);
    server->start();

}


void NodeNetworkModule::stopListening() {
    stopAcceptedConnections();
    if (server != nullptr) {
        server->stop();
    }


}

void NodeNetworkModule::onStop() {

    stopListening();
    disconnectAll();
}

void NodeNetworkModule::onStart() {
    Runnable::onStart();
    listen();
}


void NodeNetworkModule::run() {
    NodeModule::run();

    //@todo find a better way to trigger onStop after run
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
//        //@todo connect to node
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

void NodeNetworkModule::disconnect(const NodeIdType id) {
    {
        getRemoteNode(id).disconnect();
    }
//    purgeInactiveConnections();
}

void NodeNetworkModule::disconnectAll() {
//    std::lock_guard<std::mutex> g(activeConnectionsMutex);
//    //activeClientConnections.remove_if([](auto i) { return true; });
//    for (auto &&item : activeClientConnections) {
//        if (item->connection != nullptr) {
//            item->connection->shutdown();
//        }
//
//    }
//    activeClientConnections.clear();

}

const std::shared_ptr<IProtocol> &NodeNetworkModule::getProtocol() const {
    return protocol;
}

std::shared_ptr<NetworkInfo> &NodeNetworkModule::getNetworkInfo() {
    return networkInfo;
}

void NodeNetworkModule::SubModule::setupPacketProcessing(NodeNetworkModule &node) {

    for (const auto &processingItem : processingList) {
        processingItem->registerPacketProcessor(node);
    }

}
