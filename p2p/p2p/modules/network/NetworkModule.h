//
// Created by stilgar on 07.11.17.
//

#ifndef BASYCO_NETWORKMODULE_H
#define BASYCO_NETWORKMODULE_H


#include <p2p/node/module/NodeModule.h>
#include <p2p/dependency/DependencyManaged.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/TCPServer.h>

#include <p2p/modules/network/protocol/protocol/GravitonProtocol.h>
#include <p2p/modules/network/protocol/connection/IServerConnection.h>
#include <p2p/modules/network/protocol/broadcast/BroadcastScope.h>
#include <p2p/modules/network/network/NetworkInfo.h>
#include <p2p/modules/network/remote/RemoteNode.h>
#include <p2p/role/RoleScope.h>
#include <logic/events/LogicStateEvent.h>
#include <p2p/modules/network/protocol/logic/conditions/NetworkConditions.h>
#include <p2p/modules/network/remote/exception/RemoteNodeConnectionException.h>
#include "p2p/modules/network/remote/exception/RemoteNodeNotFoundException.h"


struct NodeConnectionInfo {
    ConnectionPtr connection;
    std::optional<NodeIdType> nodeId;
};

typedef std::shared_ptr<NodeConnectionInfo> NodeConnectionInfoPtr;


class NetworkModule
        : public NodeModuleDependent<NetworkModule>, public Connection::Observer/*for server connections*/ {

public:

    class Configuration {
    private:
        unsigned short port = 6667;
    public:
        unsigned short getPort() const {
            return port;
        }

        void setPort(unsigned short port) {
            Configuration::port = port;
        }

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar & port;
        }


    private:


        friend class cereal::access;
    };
    template<typename PacketType>
    using ProcessorType = std::function<typename PacketType::Response::Ptr(typename PacketType::Request::Ptr)>;

    class PacketProcessingData {

    protected:
        virtual void registerPacketProcessor(NetworkModule &node) = 0;

        friend class NetworkModule;

    };

    template<typename PacketType>
    class SpecificPacketProcessingData : public PacketProcessingData {

    private:
        ProcessorType<PacketType> processor;

        void registerPacketProcessor(NetworkModule &node) override {
            node.when(NetworkConditions::packetReceived<typename PacketType::Request>()).fireNewAction(
                    [processor = processor](const SpecificPacketEvent<typename PacketType::Request> packetEvent) {
                        //@todo Status::ERROR handling.
                        auto response = processor(packetEvent.getPacket());
                        if (response != nullptr) {
                            response->setId(packetEvent.getPacket()->getId());
                            packetEvent.getConnection()->send(response);
                        } else {
                            auto error = PacketType::Error::getNew();
                            error->setId(packetEvent.getPacket()->getId());
                            packetEvent.getConnection()->send(error);
                        }
                    });
        }

    public:
        explicit SpecificPacketProcessingData(const ProcessorType<PacketType> &processor) : processor(processor) {}


    };

    class SubModule {
        std::list<std::shared_ptr<PacketProcessingData>> processingList;
    public:
        template<typename PacketType>
        void registerPacketProcessor(ProcessorType<PacketType> processor) {
            auto data = std::make_shared<SpecificPacketProcessingData<PacketType>>(processor);
            processingList.push_back(data);
        }

    private:
        void setupPacketProcessing(NetworkModule &node);

        friend class NetworkModule;
    };





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
    explicit NetworkModule(INode &node);

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

    void onStop() override;

    void onStart() override;

    RemoteNode &connectTo(const NetAddressType &address) {
        RemoteNode &remoteNode = getRemoteNode();
        if (!remoteNode.connectTo(address)) {
            throw RemoteNodeConnectionException();
        }
        return remoteNode;
    }

    RemoteNode &connectToNode(const NodeIdType& nodeId) {
        //@todo catch exception
        RemoteNode &remoteNode = getRemoteNode(nodeId);
        remoteNode.connect();
        return remoteNode;
    }

    void disconnect(const NodeIdType &id);

    void disconnectAll();

//    //@todo add version that uses protocol and returns future
//    bool sendPacketToNode(const NodeIdType &nodeId, BasePacketPtr packet);

    template<enum Status status = Status::RESPONSE, typename SendType>
    auto sendPacketToNode(const NodeIdType &nodeId, NetworkPacketPointer<SendType> p) {
        return getRemoteNode(nodeId).sendRequestToNode(p);
    }

    template<typename SendType>
    void broadcastPacket(NetworkPacketPointer<SendType> p, const BroadcastScope &scope = BroadcastScope::CONNECTED) {
        std::lock_guard<std::mutex> g(activeConnectionsMutex);

        for (auto &&item : remoteNodes) {
            if (item->getNodeId()) {
                p->resetId();
                item->sendPacketToNode(p);
            }
        }

    }

    template<enum Status status = Status::RESPONSE, typename SendType>
    auto broadcastRequest(NetworkPacketPointer<SendType> p, const BroadcastScope &scope = BroadcastScope::CONNECTED) {
        typedef typename PacketInfo<typename SendType::BaseType, status>::Type ReturnType;

        std::map<NodeIdType, NetworkPacketPointer<ReturnType>> ret;
//        auto MAX_WAIT_TIME = 2s;

        std::lock_guard<std::mutex> g(activeConnectionsMutex);

        for (auto &&item : remoteNodes) {
            if (item->getNodeId()) {
                //id has to be reset so no two packets sent have the same id.
                p->resetId();
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
    }

    void run() override;

    const std::shared_ptr<IProtocol> &getProtocol() const;

    void update(Connection &connection, ConnectionState type) override;

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

    void testingMethod2(const LogicStateEvent<NetworkModule, int> &event) {
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
        auto activeContext = Context::getActiveContext();
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

//CONFIG_NAME(NetworkModule::Configuration, "network")

//CEREAL_REGISTER_TYPE(NetworkModule::Configuration)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(IConfig,NetworkModule::Config);

#endif //BASYCO_NETWORKMODULE_H