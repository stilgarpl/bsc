//
// Created by stilgar on 05.09.17.
//

#ifndef BASYCO_IPROTOCOL_H
#define BASYCO_IPROTOCOL_H

#include <p2p/network/protocol/packet/info/PacketInfo.h>
#include <future>
#include <p2p/network/protocol/connection/Connection.h>
#include <p2p/network/protocol/logic/events/PacketEvent.h>
#include <p2p/logic/ILogicModule.h>
#include <p2p/network/protocol/logic/events/ConnectionEvent.h>
#include <p2p/network/protocol/logic/events/ConnectionEvent.h>

#include "p2p/logic/events/Tick.h"
#include "p2p/logic/LogicManager.h"

class IProtocol;

using namespace std::placeholders;

class ProtocolWrapper {
private:
    ConnectionPtr connection;
    IProtocol *protocol;

public:
    ProtocolWrapper(ConnectionPtr connection, IProtocol *protocol);

    std::future<BasePacketPtr> send(BasePacketPtr p);
};


class IProtocol : public LogicObject {
public:

    enum class Actions {
        onPacketSent,
        onPacketReceived,
        onWork,
    };

    IProtocol(LogicManager &logicManager);

    ProtocolWrapper wrap(ConnectionPtr conn) {
        return ProtocolWrapper(conn, this);
    }

    virtual void onPacketSent(const PacketEvent &event)= 0;

    virtual void onPacketReceived(const PacketEvent &event)= 0;

    virtual void onConnectionEvent(const ConnectionEvent &event) =0;

    virtual void work(const Tick &tick)= 0;
    //virtual void send(Connection *conn, BasePacketPtr p)= 0;

    virtual std::future<BasePacketPtr> send(Connection *conn, BasePacketPtr p, const Status &expectedStatus) =0;


    template<enum Status status = Status::RESPONSE, typename SendType>
    auto sendExpect(Connection *conn, NetworkPacketPointer<SendType> p) {
        typedef typename PacketInfo<typename SendType::BaseType, status>::Type ReturnType;
        auto future = send(conn, p, status);
        //future.wait();
        ///@todo check if there is a way to do it without exceptions, maybe .valid() ?
        try {
            auto ret = future.get();

            // auto retStatus = ret->getStatus(); //debug
            if (ret->getStatus() == status) {
                return std::static_pointer_cast<ReturnType>(ret);
            } else {
                return std::shared_ptr<ReturnType>(nullptr);
            }
        } catch (const std::future_error &e) {
            //broken promise
            LOGGER(std::string("broken promise! ") + e.what());
            return std::shared_ptr<ReturnType>(nullptr);
        }
    }


    template<typename SendType>
    auto sendExpectExtended(Connection *conn, NetworkPacketPointer<SendType> p) {
        typedef typename PacketInfo<typename SendType::BaseType, Status::RESPONSE>::Type ResponseType;
        typedef typename PacketInfo<typename SendType::BaseType, Status::ERROR>::Type ErrorType;
        struct ReturnValue {
            std::shared_ptr<ResponseType> response = nullptr;
            std::shared_ptr<ErrorType> error = nullptr;
        };
        ReturnValue returnValue;
        auto future = send(conn, p, Status::RESPONSE);
        //future.wait();
        ///@todo check if there is a way to do it without exceptions, maybe .valid() ?
        try {
            auto ret = future.get();

            // auto retStatus = ret->getStatus(); //debug
            if (ret->getStatus() == Status::RESPONSE) {
                returnValue.response = std::static_pointer_cast<ResponseType>(ret);
            } else {
                returnValue.error = std::static_pointer_cast<ErrorType>(ret);
            }
            return returnValue;
        } catch (const std::future_error &e) {
            //broken promise
            return returnValue;
        }
    }


    void setupActions(ILogicModule::SetupActionHelper &actionHelper) override;

    bool assignActions(ILogicModule::AssignActionHelper &actionHelper) override;

    bool setupSources(ILogicModule::SetupSourceHelper &sourceHelper) override;

};

class DummyProtocol : public IProtocol {
    void onPacketSent(const PacketEvent &event) override;

    void onPacketReceived(const PacketEvent &event) override;

    void work(const Tick &tick) override;

public:
    DummyProtocol(LogicManager &logicManager);

    std::future<BasePacketPtr> send(Connection *conn, BasePacketPtr p, const Status &expectedStatus) override;

    void onConnectionEvent(const ConnectionEvent &event) override;

};

#endif //BASYCO_IPROTOCOL_H
