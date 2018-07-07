//
// Created by stilgar on 06.07.18.
//

#ifndef BASYCO_REMOTENODE_H
#define BASYCO_REMOTENODE_H

#include <p2p/network/protocol/connection/ClientConnection.h>
#include <Poco/Net/SocketAddress.h>
#include <p2p/network/node/NodeInfo.h>

/**
 * a representation of remote nodes.
 * manages connections with specific node and maintains node context for them.
 */
class RemoteNode {
private:
    std::optional<NodeInfo> nodeInfo;
    std::shared_ptr<ClientConnection> connection;
    Context _context;

public:
    const std::optional<NodeIdType> getNodeId() const;

    bool connectTo(const std::string &address);

    Context &context() { return _context; };

    RemoteNode();

    void disconnect() {
        if (connection != nullptr) {
            connection->shutdown();
        }
        connection = nullptr;
    }

};


#endif //BASYCO_REMOTENODE_H
