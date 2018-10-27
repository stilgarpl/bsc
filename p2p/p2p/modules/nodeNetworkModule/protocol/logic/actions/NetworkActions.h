//
// Created by stilgar on 07.10.17.
//

#ifndef BASYCO_NETWORKACTIONS_H
#define BASYCO_NETWORKACTIONS_H


#include <p2p/modules/nodeNetworkModule/protocol/logic/events/NetworkInfoEvent.h>
#include <p2p/modules/nodeNetworkModule/NodeNetworkModule.h>

class NetworkActions {

public:
    static void updateNetworkInfo(const NetworkInfoEvent &event);

    static void saveNetworkInfo(const ModuleEvent<NodeNetworkModule> &event);

    static void loadNetworkInfo(const ModuleEvent<NodeNetworkModule> &event);
};


#endif //BASYCO_NETWORKACTIONS_H
