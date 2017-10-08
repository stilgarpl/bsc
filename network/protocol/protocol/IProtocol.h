//
// Created by stilgar on 05.09.17.
//

#ifndef BASYCO_IPROTOCOL_H
#define BASYCO_IPROTOCOL_H


#include <future>
#include <network/protocol/connection/Connection.h>
#include <network/protocol/logic/events/PacketEvent.h>
#include <network/protocol/packet/info/PacketInfo.h>
#include "logic/events/Tick.h"
#include "logic/LogicManager.h"

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


class IProtocol {
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

    virtual void setupLogic(LogicManager &logicManager);

    template<typename SendType, typename ReturnType= typename PacketInfo<typename SendType::BaseType, Status::RESPONSE>::Type>
    auto sendExpect(Connection *conn, NetworkPacketPointer<SendType> p) {
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
