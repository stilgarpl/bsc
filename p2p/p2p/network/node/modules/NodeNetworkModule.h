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
#include <p2p/network/node/modules/broadcast/BroadcastScope.h>

#include <p2p/modules/configuration/IConfig.h>
#include <p2p/modules/configuration/ConfigurationModule.h>

struct NodeConnectionInfo {
    ConnectionPtr connection;
    std::experimental::optional<NodeIdType> nodeId;
};

typedef std::shared_ptr<NodeConnectionInfo> NodeConnectionInfoPtr;

class NodeNetworkModule : public NodeModuleDependent<NodeNetworkModule, ConfigurationModule> {
protected:
public:
    class Config : public IConfig {
    private:
        unsigned short port = 6667;
    public:
        unsigned short getPort() const {
            return port;
        }

        void setPort(unsigned short port) {
            Config::port = port;
        }

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar & cereal::base_class<IConfig>(this) & port;
        }


        friend class cereal::access;
    };

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
            auto[response, error] = protocol->sendExpectExtended(conn.get(), p);
            return response;

        } else {
            LOGGER("unable to send packet to " + nodeId)
            return std::shared_ptr<ReturnType>(nullptr);

        }
    };

    template<enum Status status = Status::RESPONSE, typename SendType>
    auto broadcastPacket(NetworkPacketPointer<SendType> p, const BroadcastScope &scope = BroadcastScope::CONNECTED) {
        typedef typename PacketInfo<typename SendType::BaseType, status>::Type ReturnType;

        std::map<NodeIdType, NetworkPacketPointer<ReturnType>> ret;
        auto MAX_WAIT_TIME = 2s;

        std::lock_guard<std::mutex> g(activeConnectionsMutex);

        for (auto &&item : activeClientConnections) {
            if (item->nodeId) {
                ret[*item->nodeId] = protocol->sendExpect(item->connection.get(), p);
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

    const std::unique_ptr<IProtocol> &getProtocol() const;
};


CEREAL_REGISTER_TYPE(NodeNetworkModule::Config);
//CEREAL_REGISTER_POLYMORPHIC_RELATION(IConfig,NodeNetworkModule::Config);

#endif //BASYCO_NODENETWORKMODULE_H
