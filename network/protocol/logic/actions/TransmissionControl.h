//
// Created by stilgar on 03.09.17.
//

#ifndef BASYCO_TRANSMISSIONCONTROL_H
#define BASYCO_TRANSMISSIONCONTROL_H


#include "../../../../context/Context.h"
#include "../events/PacketEvent.h"
#include "../../../../logic/events/Tick.h"
#include "../../protocol/IProtocol.h"
#include "NetworkPacketInfo.h"

///@todo move to configuration
static const Tick::clock::duration MAX_TIMEOUT = 0ms;

class TransmissionControl /*: IProtocol*/{

    std::map<NetworkPacket::IdType, std::shared_ptr<NetworkPacketInfo>> waitingPackets;

public:
    void onPacketSent(Context &, const PacketEvent &event);

    void onPacketReceived(Context &, const PacketEvent &event);

    void work(Context &, const Tick &tick);

};


#endif //BASYCO_TRANSMISSIONCONTROL_H
