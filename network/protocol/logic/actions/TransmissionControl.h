//
// Created by stilgar on 03.09.17.
//

#ifndef BASYCO_TRANSMISSIONCONTROL_H
#define BASYCO_TRANSMISSIONCONTROL_H


#include "../../../../context/Context.h"
#include "../events/PacketEvent.h"
#include "../../../../logic/events/Tick.h"

///@todo move to configuration
static const Tick::clock::duration MAX_TIMEOUT = 2500ms;

class NetworkPacketInfo {
private:
    NetworkPacketPtr packetPtr;
    Connection *connection = nullptr;
    Tick::clock::time_point timeSent;

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

};

class TransmissionControl {

    std::map<NetworkPacket::IdType, std::shared_ptr<NetworkPacketInfo>> waitingPackets;

public:
    void onPacketSent(Context &, const PacketEvent &event);

    void onPacketReceived(Context &, const PacketEvent &event);

    void work(Context &, const Tick &tick);

};


#endif //BASYCO_TRANSMISSIONCONTROL_H
