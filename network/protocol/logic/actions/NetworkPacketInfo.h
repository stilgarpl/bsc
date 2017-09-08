//
// Created by stilgar on 06.09.17.
//

#ifndef BASYCO_NETWORKPACKETINFO_H
#define BASYCO_NETWORKPACKETINFO_H

//#include "../../../../context/Context.h"
//#include "../events/PacketEvent.h"
#include <future>
#include "../../../../logic/events/Tick.h"
#include "../../packet/NetworkPacket.h"
#include "../../connection/Connection.h"


class NetworkPacketInfo {
private:
    NetworkPacketPtr packetPtr;
    Connection *connection = nullptr;
    //@todo is Tick reference necessary? maybe some global clock function? or just std::steady_clock ?
    Tick::clock::time_point timeSent;
    std::promise<NetworkPacketPtr> responsePromise;

public:
    const NetworkPacketPtr &getPacketPtr() const;

    void setPacketPtr(const NetworkPacketPtr &packetPtr);

    const std::chrono::time_point<Tick::clock> &getTimeSent() const;

    void setTimeSent(const std::chrono::time_point<Tick::clock> &timeSent);

public:
    NetworkPacketInfo(const NetworkPacketPtr &packetPtr, const std::chrono::time_point<Tick::clock> &timeSent);

    NetworkPacketInfo(const NetworkPacketPtr &packetPtr, Connection *connection,
                      const std::chrono::time_point<Tick::clock> &timeSent);

    Connection *getConnection() const;

    void setConnection(Connection *connection);

    ///@todo or simply get future?
    std::promise<NetworkPacketPtr> &getResponsePromise();

};


#endif //BASYCO_NETWORKPACKETINFO_H
