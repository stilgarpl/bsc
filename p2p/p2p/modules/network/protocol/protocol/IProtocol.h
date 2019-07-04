#include <utility>

//
// Created by stilgar on 05.09.17.
//

#ifndef BASYCO_IPROTOCOL_H
#define BASYCO_IPROTOCOL_H

#include <p2p/modules/network/protocol/packet/info/PacketInfo.h>
#include <future>
#include <p2p/modules/network/protocol/connection/Connection.h>
#include <p2p/modules/network/protocol/logic/events/PacketEvent.h>
#include <p2p/node/module/logic/ILogicModule.h>
#include <p2p/modules/network/protocol/logic/events/ConnectionEvent.h>
#include <p2p/modules/network/protocol/logic/events/ConnectionEvent.h>

#include "logic/events/Tick.h"
#include "logic/LogicManager.h"

class IProtocol;

using namespace std::placeholders;

class ProtocolWrapper {
private:
    ConnectionPtr connection;
    //@todo make a &reference?
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
        onPacketDropped,
        onWork,
    };

    explicit IProtocol(LogicManager &logicManager);

    ProtocolWrapper wrap(ConnectionPtr conn) {
        return ProtocolWrapper(std::move(conn), this);
    }

    virtual void onPacketSent(const PacketEvent &event)= 0;

    virtual void onPacketReceived(const PacketEvent &event)= 0;

    virtual void onConnectionEvent(const ConnectionEvent &event) =0;

    virtual void work(const Tick &tick)= 0;
    //virtual void send(Connection *conn, BasePacketPtr p)= 0;

    virtual std::future<BasePacketPtr>
    send(Connection *conn, BasePacketPtr p, const Status &expectedStatus) = 0;

    void testMethod(std::string);

    std::future<BasePacketPtr> send(Connection *conn, BasePacketPtr p);

    template<enum Status status = Status::RESPONSE, typename SendType>
    auto sendExpect(Connection *conn, NetworkPacketPointer<SendType> p) {
        typedef typename PacketInfo<typename SendType::BaseType, status>::Type ReturnType;
        auto future = send(conn, p, status);
        //future.wait();
        //@todo check if there is a way to do it without exceptions, maybe .valid() ?
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
        //@todo check if there is a way to do it without exceptions, maybe .valid() ?
        try {
//            LOGGER("getting future " + std::to_string(p->getId()))
            auto ret = future.get();
//            LOGGER("future got" + std::to_string(p->getId()))
            // auto retStatus = ret->getStatus(); //debug
            if (ret->getStatus() == Status::RESPONSE) {
                returnValue.response = std::static_pointer_cast<ResponseType>(ret);
            } else {
                returnValue.error = std::static_pointer_cast<ErrorType>(ret);
            }
            return returnValue;
        } catch (const std::future_error &e) {
            //broken promise
            LOGGER("future error. broken promise! returning default value (null) " + std::to_string(p->getId()))
            ERROR(std::string("Promise broken ") + e.what() + " ... " + e.code().message());
            return returnValue;
        }
    }


    void setupActions(ILogicModule::SetupActionHelper &actionHelper) override;

    bool assignActions(ILogicModule::AssignActionHelper &actionHelper) override;

    bool setupSources(ILogicModule::SetupSourceHelper &sourceHelper) override;

    virtual ~IProtocol() = default;

};

class DummyProtocol : public IProtocol {
    void onPacketSent(const PacketEvent &event) override;

    void onPacketReceived(const PacketEvent &event) override;

    void work(const Tick &tick) override;

public:
    explicit DummyProtocol(LogicManager &logicManager);

    std::future<BasePacketPtr> send(Connection *conn, BasePacketPtr p, const Status &expectedStatus) override;

    void onConnectionEvent(const ConnectionEvent &event) override;

};

#endif //BASYCO_IPROTOCOL_H
