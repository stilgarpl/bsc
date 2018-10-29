#include <utility>

//
// Created by stilgar on 06.07.18.
//

#ifndef BASYCO_REMOTENODE_H
#define BASYCO_REMOTENODE_H

#include <p2p/modules/nodeNetworkModule/protocol/connection/ClientConnection.h>
#include <Poco/Net/SocketAddress.h>
#include <p2p/node/NodeInfo.h>
#include <p2p/modules/nodeNetworkModule/protocol/protocol/IProtocol.h>
#include <p2p/modules/nodeNetworkModule/network/RemoteNodeInfo.h>
#include "p2p/modules/nodeNetworkModule/remote/exception/NotRequestException.h"

/**
 * a representation of remote nodes.
 * manages connections with specific node and maintains node context for them.
 */
class RemoteNode {
private:
    RemoteNodeInfo remoteNodeInfo;
    std::shared_ptr<Connection> connection = nullptr;
    Context::OwnPtr _context = Context::makeContext();
    std::shared_ptr<IProtocol> protocol;

public:
    const std::optional<NodeIdType> getNodeId() const;

    bool connectTo(const NetAddressType &address);

    void connect(std::shared_ptr<Connection> existingConnection) {
        ///@todo what is this function for again? don't remember why I added it
        connection = std::move(existingConnection);
    }

    bool connect();

    Context::Ptr context() { return _context; };
protected:
    RemoteNode();

public:
    void disconnect() {
        if (connection != nullptr) {
            connection->shutdown();
        }
        connection = nullptr;
    }

    bool isConnected() {
        //@todo refine to check actual connection state if that's possible?
        return connection != nullptr;
    }

    explicit RemoteNode(const std::shared_ptr<IProtocol> &protocol);

    template<typename SendType>
    void sendPacketToNode(NetworkPacketPointer<SendType> p) {
        if (connection != nullptr) {

//            LOGGER("sending packet to node " + nodeId)
            protocol->send(connection.get(), p);

        } else {
            LOGGER("unable to send packet to " +
                   (remoteNodeInfo.getNodeInfo() ? remoteNodeInfo.getNodeInfo()->getNodeId() : "remote node.") +
                   "Not connected")

        }
    };

    template<enum Status status = Status::RESPONSE, typename SendType>
    auto sendRequestToNode(NetworkPacketPointer<SendType> p) {
        typedef typename PacketInfo<typename SendType::BaseType, status>::Type ReturnType;
        if (p->getStatus() != Status::REQUEST) {
            throw NotRequestException();
        }
        if (connection != nullptr) {

//            LOGGER("sending packet to node " + nodeId)
            auto[response, error] = protocol->sendExpectExtended(connection.get(), p);
            //@todo error handling
            return response;

        } else {
            LOGGER("unable to send packet to " +
                   (remoteNodeInfo.getNodeInfo() ? remoteNodeInfo.getNodeInfo()->getNodeId() : "remote node.") +
                   "Not connected")
            return std::shared_ptr<ReturnType>(nullptr);

        }
    };

    void setNodeInfo(const NodeInfo &ni);

    const RemoteNodeInfo &getRemoteNodeInfo() const;

    void setRemoteNodeInfo(const RemoteNodeInfo &remoteNodeInfo);

    const std::optional<NetAddressType> getAddress() {
        if (connection && connection->isActive()) {
            return connection->getAddress();
        } else {
            return std::nullopt;
        }
    }

};


#endif //BASYCO_REMOTENODE_H
