//
// Created by stilgar on 06.07.18.
//

#include <optional>
#include <p2p/node/NodeInfo.h>
#include "RemoteNode.h"
#include "RemoteNodeContext.h"
#include <Poco/Net/NetException.h>


const std::optional<NodeIdType> RemoteNode::getNodeId() const {
    if (remoteNodeInfo.getNodeInfo()) {
        return remoteNodeInfo.getNodeInfo()->getNodeId();
    } else {
        return std::nullopt;
    }
}

bool RemoteNode::connectTo(const NetAddressType &address) {

//    Poco::Net::SocketAddress socketAddress(address);
    //std::shared_ptr<Poco::Net::StreamSocket> socket = std::make_shared<Poco::Net::StreamSocket>(address);
    //@todo check for problems and handle them
    try {
//        ///@todo THIS IS BAD. It switches context for any thread that calls connect!
//        Context::setActiveContext(_context);
        auto conn = std::make_shared<ClientConnection>(address,
                                                       _context);

        conn->startSending();
        conn->startReceiving();
        connection = conn;
        return true;
    } catch (const Poco::Net::ConnectionRefusedException &) {
        //@todo connection refused in connectionSource
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

const RemoteNodeInfo &RemoteNode::getRemoteNodeInfo() const {
    return remoteNodeInfo;
}

void RemoteNode::setRemoteNodeInfo(const RemoteNodeInfo &remoteNodeInfo) {
    RemoteNode::remoteNodeInfo = remoteNodeInfo;
}

bool RemoteNode::connect() {
    LOGGER("remote node trying to connect")
    for (const auto &knownAddress : remoteNodeInfo.getKnownAddresses()) {
        LOGGER("trying address  " + knownAddress);
        bool value = connectTo(knownAddress);
        if (value) return true;
    }
    return false;
}

void RemoteNode::setNodeInfo(const NodeInfo &ni) {
    //@todo if serialization supports optional, this should be changed to optional, but for now, it's shared_ptr
    remoteNodeInfo.setNodeInfo(std::make_shared<NodeInfo>(ni));
//    LOGGER(std::string("setting node info, and the connection address is ") + connection->getAddress() + " but remembered adress is " + *address);
    //@todo shouldn't this be through logic actions? or any other way? the problem is that we have to store the connection address between creating the connection and receiving node info
    remoteNodeInfo.addKnownAddress(connection->getAddress());

}

