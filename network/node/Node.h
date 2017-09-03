//
// Created by stilgar on 31.07.17.
//

#ifndef BASYCO_NODE_H
#define BASYCO_NODE_H


#include "NodeInfo.h"
#include "../protocol/connection/ClientConnection.h"
#include "NetworkInfo.h"
#include "../service/NetworkServiceManager.h"
#include "../../configuration/IConfig.h"
#include "../../logic/SourceManager.h"
#include "../../logic/LogicManager.h"
#include <Poco/Net/ServerSocket.h>
#include <memory>
#include <Poco/Net/TCPServer.h>

class Node {

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
    std::shared_ptr<Config> configuration;
    LogicManager logicManager;
    Context nodeContext;
public:
    const std::shared_ptr<Config> &getConfiguration() const;

    void setConfiguration(const std::shared_ptr<Config> &configuration);

private:
    std::shared_ptr<Poco::Net::ServerSocket> serverSocket;
    std::shared_ptr<Poco::Net::TCPServer> server;
    NodeInfo thisNodeInfo;
    std::shared_ptr<NetworkInfo> networkInfo;// = nsm(networkInfo); //network this node belongs to @todo more than 1?
    std::list<std::shared_ptr<Connection>> activeClientConnections;
protected:
    void addActiveClientConnection(std::shared_ptr<Connection> c);

    void removeActiveClientConnection(std::shared_ptr<Connection> c);

    void work();
public:
    void listen();

    void stopListening();

    bool connectTo(const NodeInfo &nodeInfo);

    bool connectTo(const std::string &address);


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
};


CEREAL_REGISTER_POLYMORPHIC_RELATION(IConfig, Node::Config);

#endif //BASYCO_NODE_H
