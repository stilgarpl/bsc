//
// Created by stilgar on 03.08.17.
//

#ifndef BASYCO_CONNECTION_H
#define BASYCO_CONNECTION_H


#include <Poco/Net/SocketStream.h>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <p2p/modules/network/protocol/processor/ConnectionProcessor.h>
#include <p2p/modules/network/protocol/packet/BasePacket.h>
#include <p2p/role/RoleScope.h>
#include <p2p/modules/network/network/NetAddressType.h>
#include <logic/state/LogicStateMachine.h>

enum class ConnectionState {
    NEW,
    CONNECTED,
    DISCONNECTED,
};


class Connection : public RoleScope, public LogicStateMachine<Connection, ConnectionState> {

protected:
    ConnectionProcessor processor;
private:
    std::mutex sendQueueLock;
    std::mutex receiveQueueLock;
    std::queue<std::shared_ptr<BasePacket>> sendQueue;
    std::queue<std::shared_ptr<BasePacket>> receiveQueue;
    std::condition_variable sendReady;
    std::condition_variable receiveReady;
    Context::OwnPtr connectionContext = nullptr; //initialized from node context in constructor
private:

    std::unique_ptr<std::thread> sendThread;
    std::unique_ptr<std::thread> receiveThread;
    bool sending = true;
    bool receiving = true;

protected:

    void workSend(Poco::Net::StreamSocket &socket);

    void workReceive(Poco::Net::StreamSocket &socket);

    //@todo if possible try to remove this method. we should hide Poco
    virtual Poco::Net::StreamSocket &getSocket()=0;

public:
    //@todo why do those methods need socket as parameter?
    virtual void startSending(Poco::Net::StreamSocket &s);

    virtual void stopSending();

    virtual void startReceiving(Poco::Net::StreamSocket &socket);

    virtual void stopReceiving();

    virtual NetAddressType getAddress() {
        //@todo if getSocket is removed, just make it pure virtual and implement in client and server connections
        return getSocket().peerAddress().toString();
    }

    //most likely temporary address, not really useful
    virtual NetAddressType getOwnAddress() {
        return getSocket().address().toString();
    }

    bool isActive() {
        return sending && receiving;
    }

protected:

public:
    //@todo @fixme there is a problem, that sometimes last packet is not being sent. flushing doesn't help. Poco is stupid. Consider replacing poco with different lib
    void send(BasePacketPtr np);

    BasePacketPtr receive();

    explicit Connection(const Context::Ptr &context);

    ConnectionProcessor &getProcessor();

    Context::Ptr getConnectionContext();

    virtual ~Connection();

    virtual void shutdown();


};


typedef std::shared_ptr<Connection> ConnectionPtr;

#endif //BASYCO_CONNECTION_H
