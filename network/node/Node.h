//
// Created by stilgar on 31.07.17.
//

#ifndef BASYCO_NODE_H
#define BASYCO_NODE_H


#include "NodeInfo.h"

#include "NetworkInfo.h"

#include "configuration/IConfig.h"
#include "logic/SourceManager.h"
#include "logic/LogicManager.h"
#include <Poco/Net/ServerSocket.h>
#include <memory>
#include <Poco/Net/TCPServer.h>
#include <network/protocol/connection/Connection.h>
#include <network/protocol/protocol/IProtocol.h>
#include <network/protocol/protocol/GravitonProtocol.h>

///@todo separate interface so NodeInfo can include INode, and Node can include NodeInfo

class NodeActions;

struct NodeConnectionInfo {
    ConnectionPtr connection;
    std::experimental::optional<NodeIdType> nodeId;
};

typedef std::shared_ptr<NodeConnectionInfo> NodeConnectionInfoPtr;

class Node {
public:
    typedef unsigned int IdType; ///@todo replace it with a real id, hash or something

public:
    class Config : public IConfig {
    private:
        unsigned short port;
    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar & port;
        }


        friend class cereal::access;

    public:
        unsigned short getPort() const {
            return port;
        }

        void setPort(unsigned short port) {
            Config::port = port;
        }
    };

private:
    std::unique_ptr<IProtocol> protocol = std::make_unique<GravitonProtocol>();
    std::shared_ptr<Config> configuration;
    LogicManager logicManager;
    Context nodeContext;
    IdType id = generateId();

    IdType generateId() {
        //@todo implement real id generation
        static IdType i = 0;
        return i++;
    }

public:
    const std::shared_ptr<Config> &getConfiguration() const;

    void setConfiguration(const std::shared_ptr<Config> &configuration);

private:
    std::shared_ptr<Poco::Net::ServerSocket> serverSocket;
    std::shared_ptr<Poco::Net::TCPServer> server;
    NodeInfo thisNodeInfo;
    std::shared_ptr<NetworkInfo> networkInfo;// = nsm(networkInfo); //network this node belongs to @todo more than 1?
    std::list<NodeConnectionInfoPtr> activeClientConnections;

public:

    decltype(activeClientConnections) &getClientConnections() {
        return activeClientConnections;
    }

protected:
    void addActiveClientConnection(std::shared_ptr<Connection> c);

    void removeActiveClientConnection(NodeConnectionInfoPtr c);

    void work();
public:
    void listen();

    void stopListening();

    bool connectTo(const NodeInfo &nodeInfo);

    bool connectTo(const std::string &address);

    bool isConnectedTo(const NodeInfo &nodeInfo);

    void start();
    void stop();
    
    virtual ~Node();

    bool connectTo(const Poco::Net::SocketAddress &address);

    Node();

    Node(int port);

    ///@todo remove this function

    LogicManager &getLogicManager() {
        return logicManager;
    }

    NodeInfo &getNodeInfo() {
        return thisNodeInfo;
    }

    void addToNetwork(NetworkIdType networkId) {
        networkInfo = std::make_shared<NetworkInfo>();
        networkInfo->setNetworkId(networkId);
        ///@todo this shouldn't be set twice... or should it?
        thisNodeInfo.setNetworkId(networkId);
    }

    std::shared_ptr<NetworkInfo> &getNetworkInfo();


    void updateNodeConnectionInfo();

    void printConnections() {
        for (auto &&item : activeClientConnections) {
            if (item->nodeId) {
                std::cout << "Connection: NO ID" << std::endl;
            } else {
                std::cout << "Connection: " << *item->nodeId << std::endl;
            }
        }
    }

    friend class NodeActions;
};


CEREAL_REGISTER_POLYMORPHIC_RELATION(IConfig, Node::Config);

#endif //BASYCO_NODE_H
