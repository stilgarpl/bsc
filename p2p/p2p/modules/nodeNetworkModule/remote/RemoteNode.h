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
    std::optional<std::string> address;

public:
    const std::optional<NodeIdType> getNodeId() const;

    bool connectTo(const std::string &address);

    void connect(std::shared_ptr<Connection> existingConnection) {
        ///@todo what is this function for again? don't remember why I added it
        connection = std::move(existingConnection);
    }

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

    template<enum Status status = Status::RESPONSE, typename SendType>
    auto sendRequestToNode(NetworkPacketPointer<SendType> p) {
        typedef typename PacketInfo<typename SendType::BaseType, status>::Type ReturnType;
        if (p->getStatus() != Status::REQUEST) {
            throw NotRequestException();
        }
        if (connection != nullptr) {

//            LOGGER("sending packet to node " + nodeId)
            auto[response, error] = protocol->sendExpectExtended(connection.get(), p);
            return response;

        } else {
            LOGGER("unable to send packet to " +
                   (remoteNodeInfo.getNodeInfo() ? remoteNodeInfo.getNodeInfo()->getNodeId() : "remote node.") +
                   "Not connected")
            return std::shared_ptr<ReturnType>(nullptr);

        }
    };

    void setNodeInfo(const NodeInfo &ni) {
        //@todo if serialization supports optional, this should be changed to optional, but for now, it's shared_ptr
        remoteNodeInfo.setNodeInfo(std::make_shared<NodeInfo>(ni));
        //@todo shouldn't this be through logic actions? or any other way? the problem is that we have to store the connection address between creating the connection and receiving node info
        if (address) {
            remoteNodeInfo.addKnownAddress(*address);
        }
    }

};


#endif //BASYCO_REMOTENODE_H
