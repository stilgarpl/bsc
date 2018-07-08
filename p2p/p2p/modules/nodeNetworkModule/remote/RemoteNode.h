//
// Created by stilgar on 06.07.18.
//

#ifndef BASYCO_REMOTENODE_H
#define BASYCO_REMOTENODE_H

#include <p2p/network/protocol/connection/ClientConnection.h>
#include <Poco/Net/SocketAddress.h>
#include <p2p/network/node/NodeInfo.h>
#include <p2p/network/protocol/protocol/IProtocol.h>

/**
 * a representation of remote nodes.
 * manages connections with specific node and maintains node context for them.
 */
class RemoteNode {
private:
    std::optional<NodeInfo> nodeInfo;
    std::shared_ptr<ClientConnection> connection;
    Context::OwnPtr _context = Context::makeContext();
    std::shared_ptr<IProtocol> protocol;

public:
    const std::optional<NodeIdType> getNodeId() const;

    bool connectTo(const std::string &address);

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

    explicit RemoteNode(const std::shared_ptr<IProtocol> &protocol);

    template<enum Status status = Status::RESPONSE, typename SendType>
    auto sendPacketToNode(NetworkPacketPointer<SendType> p) {
        typedef typename PacketInfo<typename SendType::BaseType, status>::Type ReturnType;

        if (connection != nullptr) {

//            LOGGER("sending packet to node " + nodeId)
            auto[response, error] = protocol->sendExpectExtended(connection.get(), p);
            return response;

        } else {
            LOGGER("unable to send packet to " + (nodeInfo ? nodeInfo->getNodeId() : "remote node.") + "Not connected")
            return std::shared_ptr<ReturnType>(nullptr);

        }
    };

    void setNodeInfo(const NodeInfo &ni) {
        nodeInfo = ni;
    }

};


#endif //BASYCO_REMOTENODE_H
