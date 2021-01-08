//
// Created by Krzysztof Tulidowicz on 03.08.17.
//
//#define CEREAL_THREAD_SAFE 1

#include <p2p/modules/network/protocol/logic/sources/ConnectionSource.h>
#include <logic/context/LogicContext.h>
#include <p2p/modules/network/protocol/context/ConnectionContext.h>
#include <Poco/Exception.h>
#include <p2p/core/node/context/NodeContext.h>
#include <Poco/Net/NetException.h>
#include "ConnectionException.h"
#include "Connection.h"


using namespace std::chrono_literals;

void bsc::Connection::send(BasePacketPtr np) {
    std::lock_guard<std::mutex> g(sendQueueLock);
    // std::cout << "Adding packet ..." << std::endl;
    if (sending) {
        sendQueue.push(np);
        sendReady.notify_all();
    }

}

bsc::BasePacketPtr bsc::Connection::receive() {
    std::unique_lock<std::mutex> g(receiveQueueLock);
    while (receiveQueue.empty() && receiving) {
        // std::cout << "work::receive waiting " << receiveQueue.size() << std::endl;
        //@todo 1s? condition variable maybe?
        receiveReady.wait_for(g, 1s);
        //receiveReady.wait(g);
    }
    if (!receiving) {
        //@todo error handling, maybe throw exception?
        return nullptr;
    } else {
        auto v = receiveQueue.front();
        // std::cout << "Popping " << std::endl;
        receiveQueue.pop();
        return v;
    }

}

void bsc::Connection::workSend(Poco::Net::StreamSocket& socket) {
    Poco::Net::SocketOutputStream os(socket);
    Context::setActiveContext(getConnectionContext());
    auto lc = getConnectionContext()->get<bsc::LogicContext>();
    auto& logicManager = lc->getLogicManager();
    auto connectionSourcePtr = logicManager.getSource<ConnectionSource>();

    while (sending || !sendQueue.empty()) {
        try {
            //  std::cout << "sending " << socket.address().port() << std::endl;
            //check the queue
            std::unique_lock<std::mutex> g(sendQueueLock);
//            while (sendQueue.empty() && sending) {
//                // std::cout << "work::send waiting" << std::endl;
//                //sendReady.wait_for(g, 1s);
//                sendReady.wait_for(g, 1s);
//            }
            sendReady.wait(g, [this] { return !(sendQueue.empty() && sending); });
            while (!sendQueue.empty()) {
//                  std::cout << "work::send found packet to send" << std::endl;
                BasePacketPtr v;
                {
                    std::stringstream ss;
                    ss << std::this_thread::get_id();
//                    LOGGER("sending packet " + ss.str());
                    v = sendQueue.front();
                    {
                        cereal::BinaryOutputArchive oa(os);
                        oa << v;
                    }
//                    LOGGER("packet sent " + std::to_string(v->getId()) + " " + typeid(*v).name())

//                    LOGGER("packet sent" + std::to_string(v->getId()));
                    //@todo replace this ptr with observer pattern
                    if (connectionSourcePtr != nullptr) {
                        connectionSourcePtr->sentPacket(v, this);
                    }
                    LOGGER("packet sent.")
                    sendQueue.pop();
                }

            }
            os.flush();
//            std::this_thread::sleep_for(10ms);
//            LOGGER("sending flushed");
        } catch (const cereal::Exception& e) {
            LOGGER(" C EXCEPTION")
            LOGGER(e.what());
            //socket.close();
            stopReceiving();
            stopSending();
            //cprocessor.stop();
            // if not receiving, then it's ok!
        }
        catch (const Poco::IOException& e) {
            LOGGER(" P EXCEPTION")
            //processor.stop();
            stopReceiving();
            stopSending();
            e.displayText();

        }
        //  std::this_thread::sleep_for(400ms);
    }


    LOGGER("stopped connection workSend")

}

void bsc::Connection::workReceive(Poco::Net::StreamSocket& socket) {

    Context::setActiveContext(getConnectionContext());
    auto lc = getConnectionContext()->get<bsc::LogicContext>();
    auto& logicManager = lc->getLogicManager();
    //@todo totally replace all connection source references here with observer pattern
    auto connectionSourcePtr = logicManager.getSource<ConnectionSource>();

    Poco::Net::SocketInputStream is(socket);
    //@attention is it ok to start it here?
    processor.start();
    while (receiving) {
        cereal::BinaryInputArchive ia(is);
        /*while (socket.available() > 0)*/ {
            //  std::cout << "work::receive " << socket.address().port() << std::endl;

            try {
                while (!socket.available() && receiving /*&&
                       socket.poll(Poco::Timespan(1, 1), Poco::Net::Socket::SELECT_READ)*/) {
                    //@todo not like this
                    std::this_thread::sleep_for(1ms);
                }
                if (receiving && socket.available()) {
//                    LOGGER("receiving packet")
                    //@todo missing packets issue: if there is more than one packet in the socket stream, only the first will be read and the rest will be discarded
                    ///possible way to fix this is to implement a low level protocol buffer, that would get the bytes from cereal serializer and put them in the socket stream with additional data, like size and maybe CRC or sth.
                    ///and in the same way, it should first read the size bytes and then the rest of the message from the stream and when cereal does >> it should only give it ONE packet from the buffer at a time.

                    //int safeguard = 0;
                    while (receiving /*&& safeguard++ < 5*/) {
                        BasePacketPtr v;
                        ia >> v;

//                    ia(v);
//                    LOGGER("packet received " + std::to_string(v->getId()) + " " + typeid(*v).name())
                        {
                            std::lock_guard<std::mutex> g(receiveQueueLock);
                            receiveQueue.push(v);
                            receiveReady.notify_all();
                            //   std::cout << "work::receive qs " << receiveQueue.size() << std::endl;
                            // logic.processPacket(v);
                        }
                    }
                }

//                if (!socket.poll(Poco::Timespan(1, 1), Poco::Net::Socket::SELECT_READ)) {
//                    //socket is in a bad state, probably connection was closed.
//                    LOGGER("TIMEOUT! CLOSING CONNECTION!")
//                    stopReceiving();
//                    stopSending();
//                    //@todo trigger event?
//                }
            }
            catch (const cereal::Exception &e) {
                try {
                    socket.shutdown();
                    //socket.close();
                } catch (const Poco::IOException& e) {
                    //processor.stop();
                    if (socket.available()) {
                        socket.shutdown();
                    }
                    e.displayText();

                }

            }
            catch (const Poco::IOException& e) {
                //processor.stop();
                socket.shutdown();
                e.displayText();

            }
        }
        // std::this_thread::sleep_for(1ms);

    }
    LOGGER("stopped connection workReceive")
    processor.stop();
    connectionSourcePtr->connectionClosed(this);
}


void bsc::Connection::startSending(Poco::Net::StreamSocket& socket) {
    std::unique_lock<std::recursive_mutex> g(sendThreadLock);
    sending = true;
    //startWorker(socket);
    if (sendThread == nullptr) {
        sendThread = std::make_unique<std::thread>(&Connection::workSend, this, std::ref(socket));
    }
}

void bsc::Connection::stopSending() {
    std::unique_lock<std::recursive_mutex> g(sendThreadLock);
    sending = false;
    if (sendThread != nullptr && sendThread->joinable() && std::this_thread::get_id() != sendThread->get_id()) {
        sendReady.notify_all();
        sendThread->join();
        sendThread = nullptr;
    }

}


void bsc::Connection::startReceiving(Poco::Net::StreamSocket& socket) {
    std::unique_lock<std::recursive_mutex> g(receiveThreadLock);
    receiving = true;
//    socket.setBlocking(false);
    if (receiveThread == nullptr) {
        receiveThread = std::make_unique<std::thread>(&Connection::workReceive, this, std::ref(socket));
    }
    //  processor.start();

}

void bsc::Connection::stopReceiving() {
    std::unique_lock<std::recursive_mutex> g(receiveThreadLock);
    receiving = false;
    try {
        getSocket().shutdown();
    } catch (const Poco::Net::NetException& e) {
        LOGGER(e.what());
    }
    if (receiveThread != nullptr && receiveThread->joinable() &&
        std::this_thread::get_id() != receiveThread->get_id()) {
        receiveReady.notify_all();
        receiveThread->join();
        receiveThread = nullptr;
    }
    //

    //  processor.stop();
}

bsc::Connection::Connection(const Context::Ptr& context)
    : LogicStateMachine(makeDefinition(), *this), processor(*this), connectionContext(Context::makeContext(context)) {

    connectionContext->set<ConnectionContext, bsc::Connection&>(*this);
    setState(ConnectionState::NEW);
}

bsc::ConnectionProcessor& bsc::Connection::getProcessor() {
    return processor;
}

bsc::Context::Ptr bsc::Connection::getConnectionContext() {
    return connectionContext;
}

bsc::Connection::~Connection() {

    //   LOGGER("Closing connection")
//    shutdown();


}

void bsc::Connection::shutdown() {
    stopReceiving();
    stopSending();
    processor.stop();
    processor.join();
}

bsc::NetAddressType bsc::Connection::getAddress() {
    //@todo if getSocket is removed, just make it pure virtual and implement in client and server connections
    try {
        return getSocket().peerAddress().toString();
    } catch (const Poco::Net::NetException& e) {
        using namespace std::string_literals;
        ERROR("Network exception from POCO: "s + e.what());
        throw ConnectionException("Connection Exception: Can't get connection address");
    }
}
bsc::Connection::DefinitionPtr bsc::Connection::makeDefinition() {

    static auto ptr = std::make_shared<Definition>([]() {
        Definition definition;
        definition.addState(ConnectionState::NEW, ConnectionState::CONNECTED, ConnectionState::DISCONNECTED);
        definition.addLink(ConnectionState::NEW, ConnectionState::CONNECTED, ConnectionState::DISCONNECTED);
        definition.addLink(ConnectionState::CONNECTED, ConnectionState::DISCONNECTED);
        definition.addLink(ConnectionState::DISCONNECTED, ConnectionState::CONNECTED);
        return definition;
    }());
    return ptr;
}
