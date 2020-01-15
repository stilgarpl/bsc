//
// Created by stilgar on 06.10.17.
//

#ifndef BASYCO_NETWORKSOURCE_H
#define BASYCO_NETWORKSOURCE_H


#include <p2p/modules/network/protocol/logic/events/NetworkInfoEvent.h>

#include "logic/sources/EventQueueSource.h"


class NetworkSource : public bsc::EventQueueSource<NetworkInfoEvent> {

public:
    NetworkSource(bsc::SourceManager& sourceManager);

    void networkInfoReceived(const NetworkInfo& networkInfo);

    void saveNetworkInfo(const NetworkInfo& networkInfo);
};


#endif //BASYCO_NETWORKSOURCE_H
