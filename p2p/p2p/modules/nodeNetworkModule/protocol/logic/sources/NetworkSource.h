//
// Created by stilgar on 06.10.17.
//

#ifndef BASYCO_NETWORKSOURCE_H
#define BASYCO_NETWORKSOURCE_H


#include <p2p/modules/nodeNetworkModule/protocol/logic/events/NetworkInfoEvent.h>

#include "logic/sources/EventQueueSource.h"


class NetworkSource : public EventQueueSource<NetworkInfoEvent> {

public:
    NetworkSource(SourceManager &sourceManager);

    void networkInfoReceived(const NetworkInfo &networkInfo);

    void saveNetworkInfo(const NetworkInfo &networkInfo);
};


#endif //BASYCO_NETWORKSOURCE_H
