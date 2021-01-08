//
// Created by Krzysztof Tulidowicz on 03.08.17.
//

#ifndef BSC_CONNECTION_H
#define BSC_CONNECTION_H


#include <Poco/Net/SocketStream.h>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <p2p/modules/network/protocol/processor/ConnectionProcessor.h>
#include <p2p/modules/network/protocol/packet/BasePacket.h>
#include <p2p/core/role/RoleScope.h>
#include <p2p/modules/network/network/NetAddressType.h>
#include <logic/state/LogicStateMachine.h>
#include <logic/state/DirectNotify.h>


enum class ConnectionState {
    NEW,
    CONNECTED,
    DISCONNECTED,
};


namespace bsc {
    class Connection : public RoleScope, public LogicStateMachine<Connection, ConnectionState, DirectNotify> {

    protected:
        ConnectionProcessor processor;

    private:
        std::mutex sendQueueLock;
        std::mutex receiveQueueLock;
        std::queue<std::shared_ptr<BasePacket>> sendQueue;
        std::queue<std::shared_ptr<BasePacket>> receiveQueue;
        std::condition_variable sendReady;
        std::condition_variable receiveReady;
        Context::OwnPtr connectionContext;// initialized from node context in constructor
    private:
        std::unique_ptr<std::thread> sendThread;
        std::unique_ptr<std::thread> receiveThread;
        std::recursive_mutex sendThreadLock;
        std::recursive_mutex receiveThreadLock;
        std::atomic<bool> sending   = true;
        std::atomic<bool> receiving = true;

    protected:
        void workSend(Poco::Net::StreamSocket& socket);

        void workReceive(Poco::Net::StreamSocket& socket);

        //@todo if possible try to remove this method. we should hide Poco
        virtual Poco::Net::StreamSocket& getSocket() = 0;

    private:
        static DefinitionPtr makeDefinition();

    public:
        //@todo why do those methods need socket as parameter?
        virtual void startSending(Poco::Net::StreamSocket& s);

        virtual void stopSending();

        virtual void startReceiving(Poco::Net::StreamSocket& socket);

        virtual void stopReceiving();

        virtual NetAddressType getAddress();

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

        explicit Connection(const Context::Ptr& context);

        ConnectionProcessor& getProcessor();

        Context::Ptr getConnectionContext();

        virtual ~Connection();

        virtual void shutdown();


    };

    typedef std::shared_ptr<bsc::Connection> ConnectionPtr;
}


#endif //BSC_CONNECTION_H
