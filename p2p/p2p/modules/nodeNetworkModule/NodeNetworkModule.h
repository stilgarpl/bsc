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

#include <p2p/modules/configuration/IConfig.h>
#include <p2p/modules/configuration/ConfigurationModule.h>
#include <p2p/modules/nodeNetworkModule/remote/RemoteNode.h>
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
protected:
public:


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

    RemoteNode &connectTo(const std::string &address) {
        RemoteNode &remoteNode = getRemoteNode();
        remoteNode.connectTo(address);
        return remoteNode;
    }

    void disconnect(const NodeIdType id);

    void disconnectAll();

//    ///@todo add version that uses protocol and returns future
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
//            ///@todo connect to node
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

    template<enum Status status = Status::RESPONSE, typename SendType>
    auto broadcastPacket(NetworkPacketPointer<SendType> p, const BroadcastScope &scope = BroadcastScope::CONNECTED) {
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
        ///@todo maybe return some class, not just a map?
        return ret;

        ///@todo implement faster broadcast, like below. I'm too sleepy to do it now
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
            throw RemoteNodeNotFoundException();
        }
    }
};


CEREAL_REGISTER_TYPE(NodeNetworkModuleConfig);
//CEREAL_REGISTER_POLYMORPHIC_RELATION(IConfig,NodeNetworkModule::Config);

#endif //BASYCO_NODENETWORKMODULE_H
