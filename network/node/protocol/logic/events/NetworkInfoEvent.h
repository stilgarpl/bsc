//
// Created by stilgar on 06.10.17.
//

#ifndef BASYCO_NETWORKINFOEVENT_H
#define BASYCO_NETWORKINFOEVENT_H


#include "../../../../../logic/IEvent.h"
#include "../../../NetworkInfo.h"

class NetworkInfoEvent : public IEvent<int> {
private:
    NetworkInfo networkInfo;

public:
    const NetworkInfo &getNetworkInfo() const;

    void setNetworkInfo(const NetworkInfo &networkInfo);
};


#endif //BASYCO_NETWORKINFOEVENT_H
