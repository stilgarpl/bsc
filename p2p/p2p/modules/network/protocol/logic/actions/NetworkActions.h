//
// Created by stilgar on 07.10.17.
//

#ifndef BSC_NETWORKACTIONS_H
#define BSC_NETWORKACTIONS_H


#include <p2p/modules/network/protocol/logic/events/NetworkInfoEvent.h>
#include <p2p/modules/network/NetworkModule.h>


namespace bsc {
    class NetworkActions {

    public:
        static void updateNetworkInfo(const NetworkInfoEvent& event);

        static void saveNetworkInfo(const ModuleEvent<NetworkModule>& event);

        static void loadNetworkInfo(const ModuleEvent<NetworkModule>& event);

        static void broadcastPacket(BasePacketPtr packet);
    };
}


#endif //BSC_NETWORKACTIONS_H
