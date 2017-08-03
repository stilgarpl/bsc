//
// Created by stilgar on 03.08.17.
//
#define CEREAL_THREAD_SAFE 1

#include "ClientConnection.h"
#include "logic/ServerLogic.h"

using namespace std::chrono_literals;

void Connection::send(std::shared_ptr<NetworkPacket> np) {
    std::lock_guard<std::mutex> g(sendQueueLock);
    sendQueue.push(np);

}

std::shared_ptr<NetworkPacket> Connection::receive() {
    ///@todo tu nie bardzo lock, bardziej trzeba czekac na event ze w kolejce cos jest...
    std::lock_guard<std::mutex> g(receiveQueueLock);
    auto v = receiveQueue.front();
    receiveQueue.pop();
    return v;
}

void Connection::workSend() {


}

void Connection::workReceive() {


}


void Connection::startSending(Poco::Net::StreamSocket &socket) {

    sending = true;
    startWorker(socket);
}

void Connection::stopSending() {
    sending = false;


}


void Connection::startWorker(Poco::Net::StreamSocket &socket) {
    if (workerThread == nullptr) {
        workerThread = std::make_unique<std::thread>(&Connection::work, this, std::ref(socket));
    }
}

void Connection::startReceiving(Poco::Net::StreamSocket &socket) {
    receiving = true;

    startWorker(socket);

}

void Connection::stopReceiving() {
    receiving = false;


}

void Connection::work(Poco::Net::StreamSocket &socket) {
    Poco::Net::SocketInputStream is(socket);
    Poco::Net::SocketOutputStream os(socket);

    std::shared_ptr<NetworkPacket> v;

    ServerLogic logic;


    while (receiving || sending) {
        // std::cout << "work: " << socket.address().port() << std::endl;
        if (sending) {
            //   std::cout << "sending " << std::endl;
            //check the queue
            std::lock_guard<std::mutex> g(sendQueueLock);
            if (!sendQueue.empty()) {
                std::cout << "work::send found packet to send" << std::endl;
                v = sendQueue.front();
                {
                    cereal::BinaryOutputArchive oa(os);
                    oa << v;
                }
                os.flush();
                sendQueue.pop();

            }

        }

        ///@todo while or if?
        while (receiving && socket.available() > 0) {
            std::cout << "work::receive" << socket.address().port() << std::endl;
            cereal::BinaryInputArchive ia(is);
            ia >> v;
            {
                std::lock_guard<std::mutex> g(receiveQueueLock);
                receiveQueue.push(v);
                logic.processPacket(v);
            }

        }
        std::this_thread::sleep_for(400ms);
    }

}

void Connection::stopWorker() {

}
