//
// Created by stilgar on 06.10.17.
//

#ifndef BSC_NETWORKSOURCE_H
#define BSC_NETWORKSOURCE_H


#include <p2p/modules/network/protocol/logic/events/NetworkInfoEvent.h>

#include "logic/sources/EventQueueSource.h"


namespace bsc {
    class NetworkSource : public bsc::EventQueueSource<NetworkInfoEvent> {

    public:
        NetworkSource(SourceManager& sourceManager);

        void networkInfoReceived(const NetworkInfo& networkInfo);

        void saveNetworkInfo(const NetworkInfo& networkInfo);
    };
}


#endif //BSC_NETWORKSOURCE_H
