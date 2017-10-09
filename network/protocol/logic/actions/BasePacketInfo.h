//
// Created by stilgar on 06.09.17.
//

#ifndef BASYCO_NETWORKPACKETINFO_H
#define BASYCO_NETWORKPACKETINFO_H

//#include "context/Context.h"
//#include "events/PacketEvent.h"
#include <future>
#include <network/protocol/packet/BasePacket.h>
#include <network/protocol/connection/Connection.h>
#include "logic/events/Tick.h"


class BasePacketInfo {
private:
    BasePacketPtr packetPtr;
    Connection *connection = nullptr;
    //@todo is Tick reference necessary? maybe some global clock function? or just std::steady_clock ?
    Tick::clock::time_point timeSent;
    std::promise<BasePacketPtr> responsePromise;

public:
    const BasePacketPtr &getPacketPtr() const;

    void setPacketPtr(const BasePacketPtr &packetPtr);

    const std::chrono::time_point<Tick::clock> &getTimeSent() const;

    void setTimeSent(const std::chrono::time_point<Tick::clock> &timeSent);

public:
    BasePacketInfo(const BasePacketPtr &packetPtr, const std::chrono::time_point<Tick::clock> &timeSent);

    BasePacketInfo(const BasePacketPtr &packetPtr, Connection *connection,
                      const std::chrono::time_point<Tick::clock> &timeSent);

    Connection *getConnection() const;

    void setConnection(Connection *connection);

    ///@todo or simply get future?
    std::promise<BasePacketPtr> &getResponsePromise();

};


#endif //BASYCO_NETWORKPACKETINFO_H