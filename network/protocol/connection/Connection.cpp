//
// Created by stilgar on 03.08.17.
//
//#define CEREAL_THREAD_SAFE 1

#include "ClientConnection.h"
#include "../context/ConnectionContext.h"
#include "../context/LogicContext.h"
#include "../logic/sources/ConnectionSource.h"

using namespace std::chrono_literals;

void Connection::send(NetworkPacketPtr np) {
    std::lock_guard<std::mutex> g(sendQueueLock);
    // std::cout << "Adding packet ..." << std::endl;
    sendQueue.push(np);
    sendReady.notify_all();

}

NetworkPacketPtr Connection::receive() {
    std::unique_lock<std::mutex> g(receiveQueueLock);
    while (receiveQueue.empty()) {
        // std::cout << "work::receive waiting " << receiveQueue.size() << std::endl;
        // receiveReady.wait_for(g, 1s);
        receiveReady.wait(g);
    }
    ///clion says it's an error. clion is WRONG.
    auto v = receiveQueue.front();
    // std::cout << "Popping " << std::endl;
    receiveQueue.pop();
    return v;
}

void Connection::workSend(Poco::Net::StreamSocket &socket) {
    Poco::Net::SocketOutputStream os(socket);

    auto lc = getConnectionContext().get<LogicContext>();
    auto &logicManager = lc->getLogicManager();
    auto connectionSourcePtr = logicManager.getSource<ConnectionSource>();

    while (sending) {
        //  std::cout << "sending " << socket.address().port() << std::endl;
        //check the queue
        std::unique_lock<std::mutex> g(sendQueueLock);
        while (sendQueue.empty()) {
            // std::cout << "work::send waiting" << std::endl;
            //sendReady.wait_for(g, 1s);
            sendReady.wait(g);
        }
        while (!sendQueue.empty()) {
            //  std::cout << "work::send found packet to send" << std::endl;
            NetworkPacketPtr v;
            {
                v = sendQueue.front();
                {
                    cereal::BinaryOutputArchive oa(os);
                    oa << v;
                }
                if (connectionSourcePtr != nullptr) {
                    connectionSourcePtr->sentPacket(v, this);
                }
                sendQueue.pop();
            }

        }
        os.flush();
        //  std::this_thread::sleep_for(400ms);
    }

}

void Connection::workReceive(Poco::Net::StreamSocket &socket) {

    auto lc = getConnectionContext().get<LogicContext>();
    auto &logicManager = lc->getLogicManager();
    auto connectionSourcePtr = logicManager.getSource<ConnectionSource>();

    Poco::Net::SocketInputStream is(socket);
    ///@attention is it ok to start it here?
    processor.start();
    while (receiving) {
        cereal::BinaryInputArchive ia(is);
        /*while (socket.available() > 0)*/ {
            //  std::cout << "work::receive " << socket.address().port() << std::endl;
            NetworkPacketPtr v;

            ia >> v;
            {
                std::lock_guard<std::mutex> g(receiveQueueLock);
                receiveQueue.push(v);
                receiveReady.notify_all();
                //   std::cout << "work::receive qs " << receiveQueue.size() << std::endl;
                // logic.processPacket(v);
                if (connectionSourcePtr != nullptr) {
                    connectionSourcePtr->receivedPacket(v, this);
                }

            }
        }
        // std::this_thread::sleep_for(1ms);

    }
    processor.stop();
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
    //  processor.start();

}

void Connection::stopReceiving() {
    receiving = false;

    //  processor.stop();
}

Connection::Connection(Context &context) : processor(*this), connectionContext(context) {

    connectionContext.set<ConnectionContext, Connection &>(*this);
}

ConnectionProcessor &Connection::getProcessor() {
    return processor;
}

Context &Connection::getConnectionContext() {
    return connectionContext;
}

