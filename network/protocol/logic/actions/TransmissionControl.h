//
// Created by stilgar on 03.09.17.
//

#ifndef BASYCO_TRANSMISSIONCONTROL_H
#define BASYCO_TRANSMISSIONCONTROL_H


#include "../../../../context/Context.h"
#include "../events/PacketEvent.h"
#include "../../../../logic/events/Tick.h"

class TransmissionControl {

    std::map<NetworkPacket::IdType, NetworkPacketPtr> waitingPackets;

public:
    void onPacketSent(Context &, const PacketEvent &event);

    void onPacketReceived(Context &, const PacketEvent &event);

    void work(Context &, const Tick &tick) {
        std::clog << " TransControl::" << __func__ << std::endl;


        for (auto &&it : waitingPackets) {

        }
    }

};


#endif //BASYCO_TRANSMISSIONCONTROL_H
