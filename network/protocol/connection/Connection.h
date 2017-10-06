//
// Created by stilgar on 03.08.17.
//

#ifndef BASYCO_CONNECTION_H
#define BASYCO_CONNECTION_H


#include <sys/socket.h>
#include <Poco/Net/SocketStream.h>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "../packet/BasePacket.h"
#include "../processor/ConnectionProcessor.h"

class Connection {

protected:
    ConnectionProcessor processor;
private:
    std::mutex sendQueueLock;
    std::mutex receiveQueueLock;
    std::queue<std::shared_ptr<BasePacket>> sendQueue;
    std::queue<std::shared_ptr<BasePacket>> receiveQueue;
    std::condition_variable sendReady;
    std::condition_variable receiveReady;
    Context connectionContext;
private:

    std::unique_ptr<std::thread> sendThread;
    std::unique_ptr<std::thread> receiveThread;
    bool sending = true;
    bool receiving = true;

protected:

    void workSend(Poco::Net::StreamSocket &socket);

    void workReceive(Poco::Net::StreamSocket &socket);

public:
    //@todo why do those methods need socket as parameter?
    virtual void startSending(Poco::Net::StreamSocket &s);

    virtual void stopSending();

    virtual void startReceiving(Poco::Net::StreamSocket &socket);

    virtual void stopReceiving();

protected:

public:
    void send(NetworkPacketPtr np);

    NetworkPacketPtr receive();

    Connection(Context &context);

    ConnectionProcessor &getProcessor();

    Context &getConnectionContext();


};


typedef std::shared_ptr<Connection> ConnectionPtr;

#endif //BASYCO_CONNECTION_H
