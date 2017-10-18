//
// Created by stilgar on 03.08.17.
//
//#define CEREAL_THREAD_SAFE 1

#include <p2p/network/protocol/logic/sources/ConnectionSource.h>
#include <p2p/network/protocol/context/LogicContext.h>
#include <p2p/network/protocol/context/ConnectionContext.h>
#include <Poco/Net/NetException.h>

using namespace std::chrono_literals;

void Connection::send(BasePacketPtr np) {
    std::lock_guard<std::mutex> g(sendQueueLock);
    // std::cout << "Adding packet ..." << std::endl;
    sendQueue.push(np);
    sendReady.notify_all();

}

BasePacketPtr Connection::receive() {
    std::unique_lock<std::mutex> g(receiveQueueLock);
    while (receiveQueue.empty() && receiving) {
        // std::cout << "work::receive waiting " << receiveQueue.size() << std::endl;
        //@todo 1s? condition variable maybe?
        receiveReady.wait_for(g, 1s);
        //receiveReady.wait(g);
    }
    if (!receiving) {
        ///@todo error handling, maybe throw exception?
        return nullptr;
    } else {
        ///clion says it's an error. clion is WRONG.
        auto v = receiveQueue.front();
        // std::cout << "Popping " << std::endl;
        receiveQueue.pop();
        return v;
    }

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
        while (sendQueue.empty() && sending) {
            // std::cout << "work::send waiting" << std::endl;
            //sendReady.wait_for(g, 1s);
            sendReady.wait_for(g, 1s);
        }
        while (!sendQueue.empty()) {
            //  std::cout << "work::send found packet to send" << std::endl;
            BasePacketPtr v;
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
            BasePacketPtr v;
            try {
                while (!socket.available() && receiving &&
                       socket.poll(Poco::Timespan(1, 1), Poco::Net::Socket::SELECT_READ)) {
                    ///@todo not like this
                    std::this_thread::sleep_for(1ms);
                }
                if (receiving) {
                    ia >> v;
                    {
                        std::lock_guard<std::mutex> g(receiveQueueLock);
                        receiveQueue.push(v);
                        receiveReady.notify_all();
                        //   std::cout << "work::receive qs " << receiveQueue.size() << std::endl;
                        // logic.processPacket(v);
                        ///@todo maybe move this to processor?
                        if (connectionSourcePtr != nullptr) {
                            //   LOGGER("CONNECTION SOURCE ISNT NULL")

                            connectionSourcePtr->receivedPacket(v, this);
                        } else {
                            //    LOGGER("CONNECTION SOURCE IS NULL")
                        }

                    }
                }
            }
            catch (cereal::Exception e) {
                //socket.close();
                stopReceiving();
                stopSending();
                //cprocessor.stop();
                // if not receiving, then it's ok!
            }
            catch (Poco::Net::NetException e) {
                //processor.stop();
                stopReceiving();
                stopSending();
                e.displayText();
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
    sendReady.notify_all();

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
    //  receiveReady.notify_all();

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

Connection::~Connection() {

    //   LOGGER("Closing connection")
    stopReceiving();
    stopSending();
    if (receiveThread != nullptr) {
        receiveThread->join();
    }
    if (sendThread != nullptr) {
        sendThread->join();
    }
    processor.stop();
    processor.join();


}
