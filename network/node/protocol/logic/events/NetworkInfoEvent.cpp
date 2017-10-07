//
// Created by stilgar on 06.10.17.
//

#include "NetworkInfoEvent.h"

const NetworkInfo &NetworkInfoEvent::getNetworkInfo() const {
    return networkInfo;
}

void NetworkInfoEvent::setNetworkInfo(const NetworkInfo &networkInfo) {
    NetworkInfoEvent::networkInfo = networkInfo;
}
