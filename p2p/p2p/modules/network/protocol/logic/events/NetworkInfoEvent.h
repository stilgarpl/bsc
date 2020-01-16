//
// Created by stilgar on 06.10.17.
//

#ifndef BASYCO_NETWORKINFOEVENT_H
#define BASYCO_NETWORKINFOEVENT_H


#include <p2p/modules/network/network/NetworkInfo.h>
#include "logic/IEvent.h"


namespace bsc {
    enum class NetworInfoEventId {
        NETWORK_INFO_RECEIVED,
        SAVE_NETWORK_INFO,

    };

    class NetworkInfoEvent : public bsc::IEvent<NetworInfoEventId> {
    private:
        NetworkInfo networkInfo;

    public:
        const NetworkInfo& getNetworkInfo() const;

        void setNetworkInfo(const NetworkInfo& networkInfo);
    };


}


#endif //BASYCO_NETWORKINFOEVENT_H
