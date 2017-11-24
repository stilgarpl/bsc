//
// Created by stilgar on 07.11.17.
//

#ifndef BASYCO_NODENETWORKMODULE_H
#define BASYCO_NODENETWORKMODULE_H


#include <p2p/network/node/NodeModule.h>
#include <p2p/dependency/DependencyManaged.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/TCPServer.h>

#include <p2p/network/protocol/protocol/GravitonProtocol.h>
#include <p2p/network/protocol/connection/IServerConnection.h>

struct NodeConnectionInfo {
    ConnectionPtr connection;
    std::experimental::optional<NodeIdType> nodeId;
};

typedef std::shared_ptr<NodeConnectionInfo> NodeConnectionInfoPtr;

class NodeNetworkModule : public NodeModule, public DependencyManaged<NodeNetworkModule> {
private:
    std::unique_ptr<IProtocol> protocol = std::make_unique<GravitonProtocol>();
    std::list<NodeConnectionInfoPtr> activeClientConnections; //client side
    std::mutex acceptedConnectionsMutex;
    std::mutex activeConnectionsMutex;
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

    void removeActiveClientConnection(Connection *c);

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

    bool connectToAddress(const std::string &add);

    bool isConnectedTo(const NodeInfo &nodeInfo);

    void disconnect(const NodeIdType id);

    void disconnectAll();

//    ///@todo add version that uses protocol and returns future
//    bool sendPacketToNode(const NodeIdType &nodeId, BasePacketPtr packet);

    template<enum Status status = Status::RESPONSE, typename SendType>
    auto sendPacketToNode(const NodeIdType &nodeId, NetworkPacketPointer<SendType> p) {
        typedef typename PacketInfo<typename SendType::BaseType, status>::Type ReturnType;
        ConnectionPtr conn = nullptr;
        for (auto &&connection : activeClientConnections) {
            if (connection->nodeId && connection->nodeId == nodeId) {
                conn = connection->connection;
            }
        }
        if (conn == nullptr) {
            ///@todo connect to node
        }

        if (conn != nullptr) {

//            LOGGER("sending packet to node " + nodeId)
            return protocol->sendExpect(conn.get(), p);

        } else {
            LOGGER("unable to send packet to " + nodeId)
            return std::shared_ptr<ReturnType>(nullptr);

        }
    };

    void run() override;

    const std::unique_ptr<IProtocol> &getProtocol() const;
};


#endif //BASYCO_NODENETWORKMODULE_H
