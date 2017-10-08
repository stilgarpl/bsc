//
// Created by stilgar on 03.09.17.
//

#ifndef BASYCO_TRANSMISSIONCONTROL_H
#define BASYCO_TRANSMISSIONCONTROL_H


#include <network/protocol/logic/events/PacketEvent.h>
#include "context/Context.h"

#include "logic/events/Tick.h"

#include "NetworkPacketInfo.h"

///@todo move to configuration
static const Tick::clock::duration MAX_TIMEOUT = 5500ms;

class TransmissionControl /*: IProtocol*/{

    std::map<BasePacket::IdType, std::shared_ptr<NetworkPacketInfo>> packetsWaitingForAck;
    std::map<BasePacket::IdType, std::shared_ptr<NetworkPacketInfo>> packetsWaitingToAck;

public:
    void onPacketSent(const PacketEvent &event);

    void onPacketReceived(const PacketEvent &event);

    void work(const Tick &tick);

};


#endif //BASYCO_TRANSMISSIONCONTROL_H
