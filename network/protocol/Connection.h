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
#include "NetworkPacket.h"

class Connection {

private:
    std::mutex sendQueueLock;
    std::mutex receiveQueueLock;
    std::queue<std::shared_ptr<NetworkPacket>> sendQueue;
    std::queue<std::shared_ptr<NetworkPacket>> receiveQueue;
    std::condition_variable sendReady;
private:

    std::unique_ptr<std::thread> sendThread;
    std::unique_ptr<std::thread> receiveThread;
    std::unique_ptr<std::thread> workerThread;
    bool sending = true;
    bool receiving = true;

protected:

    void workSend(Poco::Net::StreamSocket &socket);

    void workReceive(Poco::Net::StreamSocket &socket);

    void work(Poco::Net::StreamSocket &socket);

    void startWorker(Poco::Net::StreamSocket &socket);

    void stopWorker();

public:
    virtual void startSending(Poco::Net::StreamSocket &s);

    virtual void stopSending();

    virtual void startReceiving(Poco::Net::StreamSocket &socket);

    virtual void stopReceiving();

protected:





    ///@todo pomyslec nad zmiana nazw, zeby metody ktore faktycznie wysylaja nie mylily sie z tymi ktore dodaja do kolejki
//
//    template<typename T>
//    static void send(Poco::Net::SocketStream& stream,std::shared_ptr<T> np) {
//
//        cereal::BinaryOutputArchive oa(stream);
//        oa << np;
//    };
//
//    template<typename T>
//    std::shared_ptr<T> receive(Poco::Net::SocketStream& stream) {
//        std::shared_ptr<T> np;
//        cereal::BinaryInputArchive ia(stream);
//        ia >> np;
//    }

public:
    void send(std::shared_ptr<NetworkPacket> np);

    std::shared_ptr<NetworkPacket> receive();
};


#endif //BASYCO_CONNECTION_H
