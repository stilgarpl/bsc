#include <utility>

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
    std::lock_guard<ConnectionFetcher> g(connectionFetcher);
    //@todo better check
    if (isConnected() && connectionFetcher.getConnection()->isActive()) {
        return true;
    }
//    Poco::Net::SocketAddress socketAddress(address);
    //std::shared_ptr<Poco::Net::StreamSocket> socket = std::make_shared<Poco::Net::StreamSocket>(address);
    //@todo check for problems and handle them
    try {
        SetLocalContext localContext(_context); //RAII
        auto conn = std::make_shared<ClientConnection>(address,
                                                       _context);

        conn->startSendingImpl();
        conn->startReceivingImpl();
        connectionFetcher.setConnection(conn);
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

RemoteNode::RemoteNode(std::shared_ptr<IProtocol> protocol) : protocol(std::move(protocol)) {
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
    std::lock_guard<ConnectionFetcher> g(connectionFetcher);
    //@todo if serialization supports optional, this should be changed to optional, but for now, it's shared_ptr
    remoteNodeInfo.setNodeInfo(std::make_shared<NodeInfo>(ni));
//    LOGGER(std::string("setting node info, and the connection address is ") + connection->getAddress() + " but remembered adress is " + *address);
    //@todo shouldn't this be through logic actions? or any other way? the problem is that we have to store the connection address between creating the connection and receiving node info
    remoteNodeInfo.addKnownAddress(connectionFetcher.getConnection()->getAddress());

}

void RemoteNode::disconnect() {
    std::lock_guard <ConnectionFetcher> g(connectionFetcher);
    if (connectionFetcher.getConnection() != nullptr) {
        connectionFetcher.getConnection()->shutdown();
    }
    //@todo this setting to null is a problem. it deletes connection before state events could be processed, valgrind shows invalid read/write here
    connectionFetcher.setConnection(nullptr);
}

bool RemoteNode::isConnected() {
    std::lock_guard <ConnectionFetcher> g(connectionFetcher);
    //@todo refine to check actual connection state if that's possible?
    return connectionFetcher.getConnection() != nullptr && connectionFetcher.getConnection()->isActive();
}

const std::optional <NetAddressType> RemoteNode::getAddress() {
    std::lock_guard <ConnectionFetcher> g(connectionFetcher);
    if (connectionFetcher.getConnection() && connectionFetcher.getConnection()->isActive()) {
        return connectionFetcher.getConnection()->getAddress();
    } else {
        return std::nullopt;
    }
}

void ConnectionFetcher::setConnection(std::shared_ptr<Connection> p) {
    std::lock_guard<std::recursive_mutex> g(connectionLock);
    if (getConnection() != nullptr) {
        getConnection()->unregisterStateObserver(*this);
    }
    connectionPtr = p;
    if (p != nullptr) {
        p->registerStateObserver(*this);
    }
}

void ConnectionFetcher::setConnection(Connection *p) {
    std::lock_guard<std::recursive_mutex> g(connectionLock);
    if (getConnection() != nullptr) {
        getConnection()->unregisterStateObserver(*this);
    }
    connectionPtr = p;
    if (p != nullptr) {
        p->registerStateObserver(*this);
    }
}

Connection *ConnectionFetcher::getConnection() {
    std::lock_guard<std::recursive_mutex> g(connectionLock);
    return std::visit(visitor(), connectionPtr);
}

void ConnectionFetcher::update(Connection &connection, ConnectionState state) {
    std::lock_guard<std::recursive_mutex> g(connectionLock);
    if (state == ConnectionState::DISCONNECTED) {
        setConnection(nullptr); //should remove observer
    }
}

ConnectionFetcher::~ConnectionFetcher() {

}
