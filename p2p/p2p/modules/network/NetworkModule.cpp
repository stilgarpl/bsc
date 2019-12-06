//
// Created by stilgar on 07.11.17.
//


#include <p2p/modules/network/protocol/logic/sources/NodeSource.h>
#include <p2p/modules/network/protocol/logic/sources/NetworkSource.h>
#include <p2p/modules/network/protocol/logic/sources/ConnectionSource.h>
#include <p2p/modules/network/protocol/logic/actions/NodeActions.h>
#include <p2p/modules/network/protocol/logic/actions/ProtocolActions.h>
#include <p2p/modules/network/protocol/logic/actions/NetworkActions.h>
#include <p2p/modules/network/protocol/connection/ClientConnection.h>
#include <Poco/Net/NetException.h>
#include "NetworkModule.h"
#include "p2p/modules/basic/BasicModule.h"
#include <Poco/Net/SocketStream.h>
#include <p2p/modules/network/protocol/connection/ServerConnection.h>
#include <p2p/node/context/NodeContext.h>
#include <p2p/modules/network/protocol/packet/KeepAlivePacket.h>
#include <p2p/modules/network/protocol/packet/NodeInfoGroup.h>
#include <logic/state/LogicStateMachine.h>
#include <logic/evaluators/CommonEvaluators.h>
#include <logic/actions/CommonActions.h>
#include <logic/chain/ChainEvaluators.h>
#include <p2p/modules/network/remote/RemoteNodeContext.h>
#include <core/io/InputOutputContext.h>


using namespace Poco::Net;

NetworkModule::NetworkModule(INode &node) : NodeModuleDependent(node, "network") {
//    setRequired<BasicModule>();
}

void NetworkModule::setupActions(ILogicModule::SetupActionHelper &actionHelper) {
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

    actionHelper.setAction<ConnectionEvent>("onConnect", ProtocolActions::onNewConnection);


}

bool NetworkModule::assignActions(ILogicModule::AssignActionHelper &actionHelper) {

    when(event<ConnectionEvent>()).fireAction("connDebug");
    when(event<ConnectionEvent>(ConnectionEvent::IdType::CONNECTION_ESTABLISHED)).fireAction("reqNoI");
    when(event<ConnectionEvent>(ConnectionEvent::IdType::CONNECTION_ACCEPTED)).fireAction("reqNoI");
    when(event<ConnectionEvent>(ConnectionEvent::IdType::CONNECTION_ESTABLISHED)).fireAction("reqNeI");
    when(event<NodeInfoEvent>(NodeInfoEvent::IdType::NODE_INFO_RECEIVED)).fireAction("upNoI");
    when(event<NetworkInfoEvent>(NetworkInfoEvent::IdType::NETWORK_INFO_RECEIVED)).fireAction("upNeI");
    //@todo upNoI powinno cos takiego robic, addKnownNode powinien byc wywolany tylko w przypadku
    when(event<NodeInfoEvent>(NodeInfoEvent::IdType::NODE_INFO_RECEIVED)).fireAction("addKnownNode");
    when(event<NodeInfoEvent>(NodeInfoEvent::IdType::NEW_NODE_DISCOVERED)).fireAction("nodeDiscovered");

    //register packet processors from submodules

    when(state<ILogicModule>(ModuleState::SUBMODULES_PREPARED).entered()).fireStateChangeReaction(
            [&](ILogicModule& module) {
                //@todo move this mechanism to NodeModule to auto collect all submodules from other modules.
                //  submodules probably have to be optional or sth.
                auto& networkSub = module.getSubModule<NetworkModule>();
                networkSub.setupPacketProcessing(*this);
            });



    //high level logic:

//    when(event<ConnectionEvent>().withId(ConnectionEvent::IdType::CONNECTION_ESTABLISHED)).

    //send keepalives - I tried using it to help with the packet loss, but it didn't work.
    when(event<Tick>(120s)).fireNewAction(
            [&, this](auto e) { this->broadcastPacket(KeepAlivePacket::Request::getNew()); });

    when(event<ModuleEvent<NetworkModule>>(ModuleState::READY)).fireNewAction(
            NetworkActions::loadNetworkInfo);

    when(event<ModuleEvent<NetworkModule>>(ModuleState::SHUTDOWN)).fireNewAction(
            NetworkActions::saveNetworkInfo);

    when(event<ModuleEvent<NetworkModule>>()).fireNewAction([](auto event) { LOGGER("module event NNM") });

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
////                                     CommonEvaluators::stack(CommonEvaluators::passthrough,ChainEvaluators::genericChainResult(stage2), CommonEvaluators::passthrough));
//                                     CommonEvaluators::stack(CommonEvaluators::unwrapEvent,
//                                                             ChainEvaluators::chainResult(stage2)));
//    stage2.fireNewGenericChainAction(CommonActions::foreachAction(testingMethod1i, list),
//                                     CommonEvaluators::foreachValue<int>());

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


//    LogicStateMachine<NetworkModule, int> intStateMachine(*this);
//    when(event < LogicStateEvent<NetworkModule, int>>
//    ()).fireModuleAction(&NetworkModule::testingMethod2);
//    when(state<NetworkModule, int>(2).entered()/*.left()*/).valid();
//    when(state<NetworkModule, int>(4).entered()/*.left()*/).fireStateChangeReaction([](auto &netMod, auto a) {
//        LOGGER("net mode test.. " + std::to_string(a) + "  " + netMod.testingMethod())
//    });//.fireModuleAction(&NetworkModule::testingMethod);
    when(event < LogicStateEvent<NetworkModule, int>>
    ()).fireNewAction([](auto event) {
        LOGGER("logic state event " + std::to_string(event.getState()) +
               (event.getMethod() == LogicStateMethod::entered ? " entered" : " left"))
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

bool NetworkModule::setupSources(ILogicModule::SetupSourceHelper &sourceHelper) {


    sourceHelper.requireSource<ConnectionSource>();
    sourceHelper.requireSource<NetworkSource>();
    sourceHelper.requireSource<NodeSource>();
    return true;
}

bool NetworkModule::setupLogic() {

    bool ret = ILogicModule::setupLogic();
    ret &= protocol->setupLogic();
    return ret;
}


void NetworkModule::updateNodeConnectionInfo() {
    //@todo this is wrong, it blocks connections until its done.
    std::lock_guard<std::mutex> g(activeConnectionsMutex);
    //   std::lock_guard<std::mutex> g1(node.getLock());
    // std::thread([&]() {
    //    //this is meant to be run from a thread
//    LOGGER("update node ")
//        for (auto &&item : activeClientConnections) {
//            LOGGER("update connection " + (item->nodeId ? *item->nodeId : " ?? "));
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

void NetworkModule::purgeDuplicateConnections() {
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

void NetworkModule::printConnections() {
    std::lock_guard<std::mutex> g(activeConnectionsMutex);
    auto& out = Context::getActiveContext()->get<InputOutputContext>()->out();
    out << "[" << node.getNodeInfo().getNodeId() << "]:" << std::to_string(acceptedConnections.size())
        << std::endl;
    for (auto&& item : remoteNodes) {
        std::string pre = "[ ]";
        if (item->isConnected()) {
            pre = "[X]";
        }
        if (!item->getNodeId()) {
            out << pre << "Connection: NO ID" << std::endl;
        } else {
            out << pre << "Connection: " << *item->getNodeId() << std::endl;
        }
    }
}

void NetworkModule::purgeInactiveConnections() {
//    std::lock_guard<std::mutex> g(activeConnectionsMutex);
//    activeClientConnections.remove_if([&](NodeConnectionInfoPtr it) -> bool {
//        return it->connection == nullptr || !it->connection->isActive();
//    });

}


void NetworkModule::removeAcceptedConnection(IServerConnection *c) {
    // LOGGER("REMOVE REMOVE REMOVE");
    std::lock_guard<std::mutex> g(acceptedConnectionsMutex);
    acceptedConnections.remove(c);
}

void NetworkModule::addAcceptedConnection(IServerConnection *c) {
    std::lock_guard<std::mutex> g(acceptedConnectionsMutex);
    acceptedConnections.push_back(c);
}

void NetworkModule::stopAcceptedConnections() {
    std::lock_guard<std::mutex> g(acceptedConnectionsMutex);
    //@todo this segfaults, why bother stopping those connections? stopping the poco server should stop them anyway.
    for (auto &&it : acceptedConnections) {
        if (it != nullptr)
            it->stop();
    }
//    acceptedConnections.erase(acceptedConnections.remove_if([](auto) { return true; }),acceptedConnections.end());
    acceptedConnections.clear();
}


void NetworkModule::listen() {
    //SocketAddress address("127.0.0.1:6777");
    if (serverSocket == nullptr) {

//        //@todo debug
//        node.getModule<ConfigurationModule>()->save("network", loadedConfig);
        serverSocket = std::make_shared<ServerSocket>(configuration().getPort());
        LOGGER(std::string("Opening port") + std::to_string(configuration().getPort()));
    }


    server = std::make_shared<TCPServer>(
            new ServerConnectionFactory([&] { return getRemoteNode().context(); }, {*this}),
            *serverSocket);
    server->start();

}


void NetworkModule::stopListening() {
    stopAcceptedConnections();
    if (server != nullptr) {
        server->stop();
    }


}

void NetworkModule::onStop() {
    stopListening();
    disconnectAll();
}

void NetworkModule::onStart() {
    Runnable::onStart();
    listen();
}


void NetworkModule::run() {
    waitForStop();
}
//
//bool NetworkModule::sendPacketToNode(const NodeIdType &nodeId, BasePacketPtr packet) {
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

void NetworkModule::disconnect(const NodeIdType &id) {
        getRemoteNode(id).disconnect();
}

void NetworkModule::disconnectAll() {
    for (const auto &remoteNode : remoteNodes) {
        remoteNode->disconnect();
    }

}

const std::shared_ptr<IProtocol> &NetworkModule::getProtocol() const {
    return protocol;
}

std::shared_ptr<NetworkInfo> &NetworkModule::getNetworkInfo() {
    return networkInfo;
}

void NetworkModule::update(Connection &connection, ConnectionState type) {
    //@todo maybe RemoteNode should be watching this...
    switch (type) {
        case ConnectionState::NEW:
            break;
        case ConnectionState::CONNECTED:
            Context::getActiveContext()->get<RemoteNodeContext>()->getRemoteNode().connect(&connection);
//            getRemoteNode().connect(&connection);
            break;
        case ConnectionState::DISCONNECTED:
            break;
    }

}

RemoteNode& NetworkModule::connectTo(const NetAddressType& address) {
    RemoteNode& remoteNode = getRemoteNode();
    if (!remoteNode.connectTo(address)) {
        throw RemoteNodeConnectionException("Unable to connect to remote node");
    }
    return remoteNode;
}

void NetworkModule::SubModule::setupPacketProcessing(NetworkModule& node) {

    for (const auto& processingItem : processingList) {
        processingItem->registerPacketProcessor(node);
    }

}
