//
// Created by Krzysztof Tulidowicz on 06.07.18.
//

#include <optional>
#include <p2p/core/node/NodeInfo.h>
#include "RemoteNode.h"
#include "RemoteNodeContext.h"
#include <Poco/Net/NetException.h>
#include <p2p/modules/network/protocol/connection/ConnectionException.h>
#include <p2p/modules/network/protocol/packet/ConnectionControl.h>
#include <utility>

namespace bsc {

    std::optional<NodeIdType> RemoteNode::getNodeId() const {
        if (remoteNodeInfo.getNodeInfo()) {
            return remoteNodeInfo.getNodeInfo()->getNodeId();
        } else {
            return std::nullopt;
        }
    }

    bool RemoteNode::connectTo(const bsc::NetAddressType& address) {
        std::unique_lock g(*connectionFetcher);
        //@todo better check
        if (isConnected() && connectionFetcher->getConnection()->isActive()) {
            return true;
        }
//    Poco::Net::SocketAddress socketAddress(address);
        //std::shared_ptr<Poco::Net::StreamSocket> socket = std::make_shared<Poco::Net::StreamSocket>(address);
        //@todo check for problems and handle them
        try {
            bsc::SetLocalContext localContext(_context); //RAII
            auto conn = std::make_shared<bsc::ClientConnection>(address,
                                                                _context);

            conn->startSendingImpl();
            conn->startReceivingImpl();
            connectionFetcher->setConnection(conn);
            return true;
        } catch (const Poco::Net::ConnectionRefusedException&) {
            //@todo connection refused in connectionSource
            LOGGER("Connection refused")
            return false;
        } catch (const Poco::InvalidArgumentException&) {
            LOGGER("Invalid connection argument")
            return false;
        }
    }

    RemoteNode::RemoteNode() : RemoteNode(nullptr) {


    }

    RemoteNode::RemoteNode(std::shared_ptr<bsc::IProtocol> protocol) : protocol(std::move(protocol)) {
        context()->set<bsc::RemoteNodeContext, RemoteNode&>(*this);
    }

    const bsc::RemoteNodeInfo& RemoteNode::getRemoteNodeInfo() const {
        return remoteNodeInfo;
    }

    void RemoteNode::setRemoteNodeInfo(const bsc::RemoteNodeInfo& remoteNodeInfo) {
        RemoteNode::remoteNodeInfo = remoteNodeInfo;
    }

    bool RemoteNode::connect() {
        LOGGER("remote node trying to connect")
        for (const auto& knownAddress : remoteNodeInfo.getKnownAddresses()) {
            LOGGER("trying address  " + knownAddress);
            bool value = connectTo(knownAddress);
            if (value) return true;
        }
        return false;
    }

    void RemoteNode::setNodeInfo(const bsc::NodeInfo& ni) {
        std::unique_lock g(*connectionFetcher);
        //@todo if serialization supports optional, this should be changed to optional, but for now, it's shared_ptr
        remoteNodeInfo.setNodeInfo(std::make_shared<bsc::NodeInfo>(ni));
//    LOGGER(std::string("setting node info, and the connection address is ") + connection->getAddress() + " but remembered adress is " + *address);
        //@todo shouldn't this be through logic actions? or any other way? the problem is that we have to store the connection address between creating the connection and receiving node info
        auto connection = connectionFetcher->getConnection();
        if (connection != nullptr) {
            try {
                remoteNodeInfo.addKnownAddress(connection->getAddress());
            } catch (const bsc::ConnectionException& e) {
                ERROR("Error while setting known address. Connection is invalid. Disconnecting.");
                disconnect();
            }
        } else {
            ERROR("CONNECTION IS NULL")
        }

    }

    void RemoteNode::disconnect() {
        std::unique_lock g(*connectionFetcher);
        if (connectionFetcher->getConnection() != nullptr) {
            //@todo make this code nicer.
            auto disconnectRequest = bsc::ConnectionControl::Request::getNew();
            disconnectRequest->setAction(bsc::ConnectionControl::Action::DISCONNECT);
            connectionFetcher->getConnection()->send(disconnectRequest);
            //@todo this waits for all packets to flush. It's bad. Try to fix actual flushing.
            std::this_thread::sleep_for(1ms);
            connectionFetcher->getConnection()->shutdown();
        }
        //@todo this setting to null is a problem. it deletes connection before state events could be processed, valgrind shows invalid read/write here
        connectionFetcher->setConnection(nullptr);
    }

    bool RemoteNode::isConnected() {
        std::unique_lock g(*connectionFetcher);
        //@todo refine to check actual connection state if that's possible?
        return connectionFetcher->getConnection() != nullptr && connectionFetcher->getConnection()->isActive();
    }

    std::optional<bsc::NetAddressType> RemoteNode::getAddress() {
        std::unique_lock g(*connectionFetcher);
        if (connectionFetcher->getConnection() && connectionFetcher->getConnection()->isActive()) {
            return connectionFetcher->getConnection()->getAddress();
        } else {
            return std::nullopt;
        }
    }

    void bsc::ConnectionFetcher::setConnection(std::shared_ptr<Connection> p) {
        std::lock_guard<std::recursive_mutex> g(connectionLock);
        if (getConnection() != nullptr) {
            getConnection()->unregisterStateObserver(*this);
        }
        connectionPtr = p;
        if (p != nullptr) {
            p->registerStateObserver(*this);
            valid = true;
        }
    }

    void bsc::ConnectionFetcher::setConnection(Connection* p) {
        std::lock_guard<std::recursive_mutex> g(connectionLock);
        if (getConnection() != nullptr) {
            getConnection()->unregisterStateObserver(*this);
        }
        connectionPtr = p;
        if (p != nullptr) {
            p->registerStateObserver(*this);
            valid = true;
        }
    }

    bsc::Connection* bsc::ConnectionFetcher::getConnection() {
        std::lock_guard<std::recursive_mutex> g(connectionLock);
        if (!valid) {
            return nullptr;
        } else {
            return std::visit(visitor(), connectionPtr);
        }
    }

    void bsc::ConnectionFetcher::update(Connection& connection, ConnectionState state) {
        std::lock_guard<std::recursive_mutex> g(connectionLock);
        if (state == ConnectionState::DISCONNECTED) {
            valid = false;
//        setConnection(nullptr); //should remove observer
        }
    }

}