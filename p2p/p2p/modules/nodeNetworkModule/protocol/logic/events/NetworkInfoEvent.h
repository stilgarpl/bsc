//
// Created by stilgar on 06.10.17.
//

#ifndef BASYCO_NETWORKINFOEVENT_H
#define BASYCO_NETWORKINFOEVENT_H


#include <p2p/modules/nodeNetworkModule/network/NetworkInfo.h>
#include "p2p/logic/IEvent.h"


enum class NetworInfoEventId {
    NETWORK_INFO_RECEIVED

};

class NetworkInfoEvent : public IEvent<NetworInfoEventId> {
private:
    NetworkInfo networkInfo;

public:
    const NetworkInfo &getNetworkInfo() const;

    void setNetworkInfo(const NetworkInfo &networkInfo);
};


#endif //BASYCO_NETWORKINFOEVENT_H
