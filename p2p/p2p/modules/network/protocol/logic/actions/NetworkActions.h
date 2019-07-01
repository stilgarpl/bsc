//
// Created by stilgar on 07.10.17.
//

#ifndef BASYCO_NETWORKACTIONS_H
#define BASYCO_NETWORKACTIONS_H


#include <p2p/modules/network/protocol/logic/events/NetworkInfoEvent.h>
#include <p2p/modules/network/NetworkModule.h>

class NetworkActions {

public:
    static void updateNetworkInfo(const NetworkInfoEvent &event);

    static void saveNetworkInfo(const ModuleEvent<NetworkModule> &event);

    static void loadNetworkInfo(const ModuleEvent<NetworkModule> &event);

    static void broadcastPacket(BasePacketPtr packet);
};


#endif //BASYCO_NETWORKACTIONS_H
