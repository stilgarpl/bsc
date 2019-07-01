#include <utility>

#include <utility>

//
// Created by stilgar on 06.07.18.
//

#ifndef BASYCO_REMOTENODE_H
#define BASYCO_REMOTENODE_H

#include <p2p/modules/network/protocol/connection/ClientConnection.h>
#include <Poco/Net/SocketAddress.h>
#include <p2p/node/NodeInfo.h>
#include <p2p/modules/network/protocol/protocol/IProtocol.h>
#include <p2p/modules/network/network/RemoteNodeInfo.h>
#include "p2p/modules/network/remote/exception/NotRequestException.h"

#include <variant>


class ConnectionFetcher : public Connection::Observer {
private:
    std::variant<std::shared_ptr<Connection>/*client connection*/, Connection */*server connection*/> connectionPtr = nullptr;
    std::recursive_mutex connectionLock;

    class visitor {
    public:
        Connection *operator()(Connection *p) {
            return p;
        }

        Connection *operator()(std::shared_ptr<Connection> p) {
            return p.get();
        }
    };

public:
    virtual ~ConnectionFetcher();


public:
    void setConnection(std::shared_ptr<Connection> p);

    void setConnection(Connection *p);

    Connection *getConnection();

    void update(Connection &connection, ConnectionState state) override;

    void lock() {
        connectionLock.lock();
    }

    void unlock() {
        connectionLock.unlock();
    }
};


/**
 * a representation of remote nodes.
 * manages connections with specific node and maintains node context for them.
 */
class RemoteNode {
private:
    RemoteNodeInfo remoteNodeInfo;
    ConnectionFetcher connectionFetcher;
    Context::OwnPtr _context = Context::makeContext();
    std::shared_ptr<IProtocol> protocol;

public:
    RemoteNode(const RemoteNode &) = delete; //no copying
    const std::optional<NodeIdType> getNodeId() const;

    bool connectTo(const NetAddressType &address);

    void connect(std::shared_ptr<Connection> existingConnection) {
        connectionFetcher.setConnection(std::move(existingConnection));
    }

    void connect(Connection *existingConnection) {
        connectionFetcher.setConnection(existingConnection);
    }

    bool connect();

    Context::Ptr context() { return _context; };
protected:
    RemoteNode();

public:
    void disconnect() {
        std::lock_guard<ConnectionFetcher> g(connectionFetcher);
        if (connectionFetcher.getConnection() != nullptr) {
            connectionFetcher.getConnection()->shutdown();
        }
        connectionFetcher.setConnection(nullptr);
    }

    bool isConnected() {
        std::lock_guard<ConnectionFetcher> g(connectionFetcher);
        //@todo refine to check actual connection state if that's possible?
        return connectionFetcher.getConnection() != nullptr;
    }

    explicit RemoteNode(std::shared_ptr<IProtocol> protocol);

    template<typename SendType>
    void sendPacketToNode(NetworkPacketPointer<SendType> p) {
        std::lock_guard<ConnectionFetcher> g(connectionFetcher);
        if (connectionFetcher.getConnection() != nullptr) {

//            LOGGER("sending packet to node " + nodeId)
            protocol->send(connectionFetcher.getConnection(), p);

        } else {
            LOGGER("unable to send packet to " +
                   (remoteNodeInfo.getNodeInfo() ? remoteNodeInfo.getNodeInfo()->getNodeId() : "remote node.") +
                   "Not connected")

        }
    }

    template<enum Status status = Status::RESPONSE, typename SendType>
    auto sendRequestToNode(NetworkPacketPointer<SendType> p) {
        std::lock_guard<ConnectionFetcher> g(connectionFetcher);
        typedef typename PacketInfo<typename SendType::BaseType, status>::Type ReturnType;
        if (p->getStatus() != Status::REQUEST) {
            throw NotRequestException();
        }
        if (connectionFetcher.getConnection() != nullptr) {

//            LOGGER("sending packet to node " + nodeId)
            auto[response, error] = protocol->sendExpectExtended(connectionFetcher.getConnection(), p);
            //@todo error handling
            return response;

        } else {
            LOGGER("unable to send packet to " +
                   (remoteNodeInfo.getNodeInfo() ? remoteNodeInfo.getNodeInfo()->getNodeId() : "remote node.") +
                   "Not connected")
            return std::shared_ptr<ReturnType>(nullptr);

        }
    }

    void setNodeInfo(const NodeInfo &ni);

    const RemoteNodeInfo &getRemoteNodeInfo() const;

    void setRemoteNodeInfo(const RemoteNodeInfo &remoteNodeInfo);

    const std::optional<NetAddressType> getAddress() {
        std::lock_guard<ConnectionFetcher> g(connectionFetcher);
        if (connectionFetcher.getConnection() && connectionFetcher.getConnection()->isActive()) {
            return connectionFetcher.getConnection()->getAddress();
        } else {
            return std::nullopt;
        }
    }

};


#endif //BASYCO_REMOTENODE_H
