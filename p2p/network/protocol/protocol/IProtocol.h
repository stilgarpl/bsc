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


class IProtocol : public ILogicModule {
public:

    enum class Actions {
        onPacketSent,
        onPacketReceived,
        onWork,
    };

    ProtocolWrapper wrap(ConnectionPtr conn) {
        return ProtocolWrapper(conn, this);
    }

    virtual void onPacketSent(const PacketEvent &event)= 0;

    virtual void onPacketReceived(const PacketEvent &event)= 0;

    virtual void work(const Tick &tick)= 0;
    //virtual void send(Connection *conn, BasePacketPtr p)= 0;

    virtual std::future<BasePacketPtr> send(Connection *conn, BasePacketPtr p) =0;

    virtual bool setupLogic(LogicManager &logicManager);

    template<enum Status status = Status::RESPONSE, typename SendType>
    auto sendExpect(Connection *conn, NetworkPacketPointer<SendType> p) {
        typedef typename PacketInfo<typename SendType::BaseType, status>::Type ReturnType;
        auto future = send(conn, p);
        return std::static_pointer_cast<ReturnType>(future.get());
    }


};

class DummyProtocol : public IProtocol {
    void onPacketSent(const PacketEvent &event) override;

    void onPacketReceived(const PacketEvent &event) override;

    void work(const Tick &tick) override;

public:
    std::future<BasePacketPtr> send(Connection *conn, BasePacketPtr p) override;


};

#endif //BASYCO_IPROTOCOL_H
