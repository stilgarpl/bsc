//
// Created by stilgar on 07.11.17.
//

#ifndef BASYCO_NODENETWORKMODULE_H
#define BASYCO_NODENETWORKMODULE_H


#include <p2p/node/NodeModule.h>
#include <p2p/dependency/DependencyManaged.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/TCPServer.h>

#include <p2p/modules/nodeNetworkModule/protocol/protocol/GravitonProtocol.h>
#include <p2p/modules/nodeNetworkModule/protocol/connection/IServerConnection.h>
#include <p2p/modules/nodeNetworkModule/protocol/broadcast/BroadcastScope.h>
#include <p2p/modules/nodeNetworkModule/network/NetworkInfo.h>
#include <p2p/modules/configuration/IConfig.h>
#include <p2p/modules/configuration/ConfigurationModule.h>
#include <p2p/modules/nodeNetworkModule/remote/RemoteNode.h>
#include <p2p/role/RoleScope.h>
#include <p2p/logic/events/LogicStateEvent.h>
#include "p2p/modules/nodeNetworkModule/remote/exception/RemoteNodeNotFoundException.h"


struct NodeConnectionInfo {
    ConnectionPtr connection;
    std::optional<NodeIdType> nodeId;
};

typedef std::shared_ptr<NodeConnectionInfo> NodeConnectionInfoPtr;

class NodeNetworkModuleConfig : public IConfig {
private:
    unsigned short port = 6667;
public:
    unsigned short getPort() const {
        return port;
    }

    void setPort(unsigned short port) {
        NodeNetworkModuleConfig::port = port;
    }

private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar & cereal::base_class<IConfig>(this) & port;
    }


private:


    friend class cereal::access;
};


CONFIG_NAME(NodeNetworkModuleConfig, "network");

class NodeNetworkModule
        : public NodeModuleConfigDependent<NodeNetworkModule, NodeNetworkModuleConfig, ConfigurationModule> {
    //@todo why is this a pointer?
    std::shared_ptr<NetworkInfo> networkInfo;
protected:
public:

    //@todo this probably shouldn't return & but ordinary shared_ptr... or just &, without the pointer.
    std::shared_ptr<NetworkInfo> &getNetworkInfo();

    void addToNetwork(const NetworkIdType &networkId) {
        networkInfo = std::make_shared<NetworkInfo>();
        networkInfo->setNetworkId(networkId);
        //@todo this shouldn't be set twice... or should it?
        node.getNodeInfo().setNetworkId(networkId);
    }

private:
    std::shared_ptr<IProtocol> protocol = std::make_shared<GravitonProtocol>(logicManager);
//    std::list<NodeConnectionInfoPtr> activeClientConnections; //client side
    std::mutex acceptedConnectionsMutex;
    std::mutex activeConnectionsMutex;
    std::list<IServerConnection *> acceptedConnections; //server side
    std::shared_ptr<Poco::Net::ServerSocket> serverSocket;
    std::shared_ptr<Poco::Net::TCPServer> server;


    std::list<std::shared_ptr<RemoteNode>> remoteNodes;
public:
    explicit NodeNetworkModule(INode &node);

    bool setupLogic() override;

    void setupActions(ILogicModule::SetupActionHelper &actionHelper) override;

    bool assignActions(ILogicModule::AssignActionHelper &actionHelper) override;

    bool setupSources(ILogicModule::SetupSourceHelper &sourceHelper) override;

    void updateNodeConnectionInfo();

    void purgeDuplicateConnections();

    void purgeInactiveConnections();

    void printConnections();

public:

//    //@todo this should not be public
//    decltype(activeClientConnections) &getClientConnections() {
//        return activeClientConnections;
//    }


private:
    void stopAcceptedConnections();

public: // @todo should be public or shouldn't ?
    void addAcceptedConnection(IServerConnection *c);

    void removeAcceptedConnection(IServerConnection *c);

    void listen();

    void stopListening();

    void onStop() override;;

    void onStart() override;

    RemoteNode &connectTo(const NetAddressType &address) {
        RemoteNode &remoteNode = getRemoteNode();
        remoteNode.connectTo(address);
        return remoteNode;
    }

    RemoteNode &connectToNode(const NodeIdType& nodeId) {
        //@todo catch exception
        RemoteNode &remoteNode = getRemoteNode(nodeId);
        remoteNode.connect();
        return remoteNode;
    }

    void disconnect(const NodeIdType id);

    void disconnectAll();

//    //@todo add version that uses protocol and returns future
//    bool sendPacketToNode(const NodeIdType &nodeId, BasePacketPtr packet);

    template<enum Status status = Status::RESPONSE, typename SendType>
    auto sendPacketToNode(const NodeIdType &nodeId, NetworkPacketPointer<SendType> p) {
        return getRemoteNode(nodeId).sendRequestToNode(p);
    };

//
//    template<enum Status status = Status::RESPONSE, typename SendType>
//    auto sendPacketToNode(const NodeIdType &nodeId, NetworkPacketPointer<SendType> p) {
//        typedef typename PacketInfo<typename SendType::BaseType, status>::Type ReturnType;
//        ConnectionPtr conn = nullptr;
//        for (auto &&connection : activeClientConnections) {
//            if (connection->nodeId && connection->nodeId == nodeId) {
//                conn = connection->connection;
//            }
//        }
//        if (conn == nullptr) {
//            //@todo connect to node
//        }
//
//        if (conn != nullptr) {
//
////            LOGGER("sending packet to node " + nodeId)
//            auto[response, error] = protocol->sendExpectExtended(conn.get(), p);
//            return response;
//
//        } else {
//            LOGGER("unable to send packet to " + nodeId)
//            return std::shared_ptr<ReturnType>(nullptr);
//
//        }
//    };

    template<typename SendType>
    void broadcastPacket(NetworkPacketPointer<SendType> p, const BroadcastScope &scope = BroadcastScope::CONNECTED) {
        //@todo can I actually send one packet multiple times ? doesn't that confuse protocol waiting for specific ids?
        auto MAX_WAIT_TIME = 2s;

        std::lock_guard<std::mutex> g(activeConnectionsMutex);

        for (auto &&item : remoteNodes) {
            if (item->getNodeId()) {
                item->sendPacketToNode(p);
            }
        }

    };

    template<enum Status status = Status::RESPONSE, typename SendType>
    auto broadcastRequest(NetworkPacketPointer<SendType> p, const BroadcastScope &scope = BroadcastScope::CONNECTED) {
        typedef typename PacketInfo<typename SendType::BaseType, status>::Type ReturnType;

        //@todo can I actually send one packet multiple times ? doesn't that confuse protocol waiting for specific ids?
        std::map<NodeIdType, NetworkPacketPointer<ReturnType>> ret;
        auto MAX_WAIT_TIME = 2s;

        std::lock_guard<std::mutex> g(activeConnectionsMutex);

        for (auto &&item : remoteNodes) {
            if (item->getNodeId()) {
                ret[*item->getNodeId()] = item->sendRequestToNode(p);
            }
        }
        //@todo maybe return some class, not just a map?
        return ret;

        //@todo implement faster broadcast, like below. I'm too sleepy to do it now
//        //@todo other scopes than connected
//        std::map<NodeIdType, std::future<BasePacketPtr>> responses;
//        {
//            std::lock_guard<std::mutex> g(activeConnectionsMutex);
//
//            for (auto &&item : activeClientConnections) {
//                if (item->nodeId) {
//                    responses[*item->nodeId] = protocol->send(item->connection.get(), p, status);
//                }
//            }
//        }
//
//        for (auto && item/*[key, item]*/ : responses) {
//            if (item.second.wait_for(0) == std::future_status::ready)
//        }
    };

    void run() override;

    const std::shared_ptr<IProtocol> &getProtocol() const;


    /**
     * this method creates new remote nodes
     * @todo remove inactive nodes older than...
     * @return new remote node that is added to the list
     */
    RemoteNode &getRemoteNode() {
        std::shared_ptr<RemoteNode> remoteNode = std::make_shared<RemoteNode>(protocol);
        remoteNode->context()->setParentContext(node.getContext());
        remoteNodes.push_back(remoteNode);
        return *remoteNode;
    }

    RemoteNode &getRemoteNode(const NodeIdType &nodeId) {
        auto iter = std::find_if(remoteNodes.begin(), remoteNodes.end(), [&](const std::shared_ptr<RemoteNode> obj) {
            return (obj->getNodeId() && obj->getNodeId() == nodeId);
        });
        if (iter != remoteNodes.end()) {
            return **iter;
        } else {
            //there is no active remote node with this id, let's see if it is known in network information
            auto networkInfo = getNetworkInfo();
            if (networkInfo->isNodeKnown(nodeId)) {
                auto &remoteNode = getRemoteNode();
                remoteNode.setRemoteNodeInfo(networkInfo->getRemoteNodeInfo(nodeId));
                return remoteNode;
            } else {
                throw RemoteNodeNotFoundException();
            }
        }
    }

    ////COMMANDS
    std::string testingMethod() {
        int a = 5;
        LOGGER("Command testing method " + std::to_string(a));
        return "aaa";
    }

    void testingMethod2(const LogicStateEvent<NodeNetworkModule, int> &event) {
        int a = event.getEventId();
        LOGGER("Command testing method " + std::to_string(a));
    }

    static Tick testingMethod3(const Tick &tick) {
        LOGGER("testing method 3!")
        return tick;
    }

    static Tick testingMethod3s(const Tick &tick, std::string s) {
        LOGGER("testing method 3! " + s)
        return tick;
    }

    static std::string testingMethod1s(std::string s) {
        LOGGER("testing method 1s ! " + s)
        return s;
    }

    static std::string testingMethod1xs(std::string s) {
        LOGGER("ret testin XXXXXXXXX 1s ! " + s)
        return s;
    }

    static void testingMethod1i(int a) {
        LOGGER("testing method 1i ! " + std::to_string(a));
    }

    static IEvent<int> testingMethod4(const IEvent<std::string> &se) {
        IEvent<int> a;
        return a;
    }

    template <typename EvenType>
    static EvenType evaluatorTest(const EvenType& e) {
        return e;
    }
};


CEREAL_REGISTER_TYPE(NodeNetworkModuleConfig);
//CEREAL_REGISTER_POLYMORPHIC_RELATION(IConfig,NodeNetworkModule::Config);

#endif //BASYCO_NODENETWORKMODULE_H
