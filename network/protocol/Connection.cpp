//
// Created by stilgar on 03.08.17.
//
#define CEREAL_THREAD_SAFE 1

#include "ClientConnection.h"
#include "logic/ServerLogic.h"

using namespace std::chrono_literals;

void Connection::send(std::shared_ptr<NetworkPacket> np) {
    std::lock_guard<std::mutex> g(sendQueueLock);
    std::cout << "Adding packet ..." << std::endl;
    sendQueue.push(np);
    sendReady.notify_all();

}

std::shared_ptr<NetworkPacket> Connection::receive() {
    ///@todo tu nie bardzo lock, bardziej trzeba czekac na event ze w kolejce cos jest...
    std::lock_guard<std::mutex> g(receiveQueueLock);
    auto v = receiveQueue.front();
    receiveQueue.pop();
    return v;
}

void Connection::workSend(Poco::Net::StreamSocket &socket) {
    Poco::Net::SocketOutputStream os(socket);

    while (sending) {
        //  std::cout << "sending " << socket.address().port() << std::endl;
        //check the queue
        std::unique_lock<std::mutex> g(sendQueueLock);
        while (sendQueue.empty()) {
            std::cout << "work::send waiting" << std::endl;
            sendReady.wait(g);
        }
        while (!sendQueue.empty()) {
            std::cout << "work::send found packet to send" << std::endl;
            std::shared_ptr<NetworkPacket> v;
            {
                v = sendQueue.front();
                {
                    cereal::BinaryOutputArchive oa(os);
                    oa << v;
                }

                sendQueue.pop();
            }

        }
        os.flush();
        //  std::this_thread::sleep_for(400ms);
    }

}

void Connection::workReceive(Poco::Net::StreamSocket &socket) {

    Poco::Net::SocketInputStream is(socket);

    ServerLogic logic;
    while (receiving) {
        cereal::BinaryInputArchive ia(is);
        /*while (socket.available() > 0)*/ {
            std::cout << "work::receive" << socket.address().port() << std::endl;
            std::shared_ptr<NetworkPacket> v;

            ia >> v;
            {
                std::lock_guard<std::mutex> g(receiveQueueLock);
                receiveQueue.push(v);
                logic.processPacket(v);
            }
        }
        // std::this_thread::sleep_for(1ms);

    }
}


void Connection::startSending(Poco::Net::StreamSocket &socket) {

    sending = true;
    //startWorker(socket);
    if (sendThread == nullptr) {
        sendThread = std::make_unique<std::thread>(&Connection::workSend, this, std::ref(socket));
    }
}

void Connection::stopSending() {
    sending = false;


}


void Connection::startReceiving(Poco::Net::StreamSocket &socket) {
    receiving = true;

    if (receiveThread == nullptr) {
        receiveThread = std::make_unique<std::thread>(&Connection::workReceive, this, std::ref(socket));
    }

}

void Connection::stopReceiving() {
    receiving = false;


}

