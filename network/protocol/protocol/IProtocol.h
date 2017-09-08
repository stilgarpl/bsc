//
// Created by stilgar on 05.09.17.
//

#ifndef BASYCO_IPROTOCOL_H
#define BASYCO_IPROTOCOL_H


#include <future>
#include "../connection/Connection.h"
#include "../../../logic/events/Tick.h"
#include "../logic/events/PacketEvent.h"
#include "../../../logic/LogicManager.h"

class IProtocol;

using namespace std::placeholders;

class ProtocolWrapper {
private:
    Connection *connection;
    IProtocol *protocol;

public:
    ProtocolWrapper(Connection *connection, IProtocol *protocol);

    std::future<NetworkPacketPtr> send(NetworkPacketPtr p);
};


class IProtocol {
public:

    enum class Actions {
        onPacketSent,
        onPacketReceived,
        onWork,
    };

    ProtocolWrapper wrap(Connection *conn) {
        return ProtocolWrapper(conn, this);
    }

    virtual void onPacketSent(Context &, const PacketEvent &event)= 0;

    virtual void onPacketReceived(Context &, const PacketEvent &event)= 0;

    virtual void work(Context &, const Tick &tick)= 0;
    //virtual void send(Connection *conn, NetworkPacketPtr p)= 0;

    virtual std::future<NetworkPacketPtr> send(Connection *conn, NetworkPacketPtr p) =0;

    virtual void setupLogic(LogicManager &logicManager);
};

class DummyProtocol : public IProtocol {
    void onPacketSent(Context &context, const PacketEvent &event) override;

    void onPacketReceived(Context &context, const PacketEvent &event) override;

    void work(Context &context, const Tick &tick) override;

public:
    std::future<NetworkPacketPtr> send(Connection *conn, NetworkPacketPtr p) override;
};

#endif //BASYCO_IPROTOCOL_H
