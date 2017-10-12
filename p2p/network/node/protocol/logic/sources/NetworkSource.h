//
// Created by stilgar on 06.10.17.
//

#ifndef BASYCO_NETWORKSOURCE_H
#define BASYCO_NETWORKSOURCE_H


#include <p2p/network/node/protocol/logic/events/NetworkInfoEvent.h>

#include "p2p/logic/sources/EventQueueSource.h"


class NetworkSource : public EventQueueSource<NetworkInfoEvent> {

public:
    void networkInfoReceived(const NetworkInfo &networkInfo);
};


#endif //BASYCO_NETWORKSOURCE_H
