//
// Created by stilgar on 06.07.18.
//

#include <optional>
#include <p2p/network/node/NodeInfo.h>
#include "RemoteNode.h"
#include "RemoteNodeContext.h"
#include <Poco/Net/NetException.h>


const std::optional<NodeIdType> RemoteNode::getNodeId() const {
    if (nodeInfo) {
        return nodeInfo->getNodeId();
    } else {
        return std::nullopt;
    }
}

bool RemoteNode::connectTo(const std::string &address) {

    Poco::Net::SocketAddress socketAddress(address);
    //std::shared_ptr<Poco::Net::StreamSocket> socket = std::make_shared<Poco::Net::StreamSocket>(address);
    //@todo check for problems and handle them
    try {
        Context::setActiveContext(_context);
        connection = std::make_shared<ClientConnection>(socketAddress,
                                                        _context);

        connection->startSending();
        connection->startReceiving();
        return true;
    } catch (const Poco::Net::ConnectionRefusedException &) {
        ///@todo connection refused in connectionSource
        return false;
    } catch (const Poco::InvalidArgumentException &) {
        return false;
    }
}

RemoteNode::RemoteNode() : RemoteNode(nullptr) {


}

RemoteNode::RemoteNode(const std::shared_ptr<IProtocol> &protocol) : protocol(protocol) {
    context()->set<RemoteNodeContext, RemoteNode &>(*this);
}

