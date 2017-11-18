//
// Created by stilgar on 07.11.17.
//

#ifndef BASYCO_NODENETWORKMODULE_H
#define BASYCO_NODENETWORKMODULE_H


#include <p2p/network/node/NodeModule.h>
#include <p2p/dependency/DependencyManaged.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/TCPServer.h>
class NodeNetworkModule : public NodeModule, public DependencyManaged<NodeNetworkModule> {
private:
    std::unique_ptr<IProtocol> protocol = std::make_unique<GravitonProtocol>();
    std::list<NodeConnectionInfoPtr> activeClientConnections; //client side
    std::mutex acceptedConnectionsMutex;
    std::list<IServerConnection *> acceptedConnections; //server side
    std::shared_ptr<Poco::Net::ServerSocket> serverSocket;
    std::shared_ptr<Poco::Net::TCPServer> server;
public:
    NodeNetworkModule(INode &node);

    bool setupLogic(LogicManager &logicManager) override;

    void setupActions(LogicManager &logicManager) override;

    bool assignActions(LogicManager &logicManager) override;

    bool setupSources(LogicManager &logicManager) override;

    void updateNodeConnectionInfo();

    void purgeDuplicateConnections();

    void purgeInactiveConnections();

    void printConnections();

    void addActiveClientConnection(std::shared_ptr<Connection> c);

    void removeActiveClientConnection(NodeConnectionInfoPtr c);

public:

    //@todo this should not be public
    decltype(activeClientConnections) &getClientConnections() {
        return activeClientConnections;
    }


private:
    void stopAcceptedConnections();

public: // @todo should be public or shouldn't ?
    void addAcceptedConnection(IServerConnection *c);

    void removeAcceptedConnection(IServerConnection *c);

    void listen();

    void stopListening();

    bool connectTo(const NodeInfo &nodeInfo);

    void onStop() override;

    bool connectTo(const Poco::Net::SocketAddress &address);;

    void onStart() override;

    bool connectTo(const std::string &address);

    bool isConnectedTo(const NodeInfo &nodeInfo);
};


#endif //BASYCO_NODENETWORKMODULE_H
