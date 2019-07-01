//
// Created by stilgar on 06.09.17.
//

#ifndef BASYCO_NETWORKPACKETINFO_H
#define BASYCO_NETWORKPACKETINFO_H

#include <future>
#include <p2p/modules/network/protocol/packet/BasePacket.h>
#include <p2p/modules/network/protocol/connection/Connection.h>
#include "logic/events/Tick.h"


class BasePacketInfo {
private:
    unsigned int retry = 0;
    BasePacketPtr packetPtr;
    Connection *connection = nullptr;
    //@todo is Tick reference necessary? maybe some global clock function? or just std::steady_clock ?
    Status expectedStatus = Status::RESPONSE; //@todo what if we expect more than one response? ERROR for example?
    Tick::clock::time_point timeSent;
    std::promise<BasePacketPtr> responsePromise;

public:
    BasePacketInfo(const BasePacketPtr &packetPtr, Connection *connection,
                   const std::chrono::time_point<Tick::clock> &timeSent);

public:
    const BasePacketPtr &getPacketPtr() const;

    void setPacketPtr(const BasePacketPtr &packetPtr);

    const std::chrono::time_point<Tick::clock> &getTimeSent() const;

    void setTimeSent(const std::chrono::time_point<Tick::clock> &timeSent);

public:
    BasePacketInfo(const BasePacketPtr &packetPtr, const std::chrono::time_point<Tick::clock> &timeSent);

    BasePacketInfo(const BasePacketPtr &packetPtr, Connection *connection,
                   const std::chrono::time_point<Tick::clock> &timeSent, Status expectedStatus);

    Connection *getConnection() const;

    void setConnection(Connection *connection);

    //@todo or simply get future?
    std::promise<BasePacketPtr> &getResponsePromise();

    Status getExpectedStatus() const;

    void setExpectedStatus(Status expectedStatus);

    unsigned int getRetry() const;

    void setRetry(unsigned int retry);

};


#endif //BASYCO_NETWORKPACKETINFO_H
