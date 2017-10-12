//
// Created by stilgar on 07.10.17.
//

#ifndef BASYCO_NETWORKACTIONS_H
#define BASYCO_NETWORKACTIONS_H


#include <p2p/network/node/protocol/logic/events/NetworkInfoEvent.h>

class NetworkActions {

public:
    static void updateNetworkInfo(const NetworkInfoEvent &event);
};


#endif //BASYCO_NETWORKACTIONS_H
