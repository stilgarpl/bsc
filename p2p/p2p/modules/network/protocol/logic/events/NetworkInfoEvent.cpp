//
// Created by stilgar on 06.10.17.
//


#include "NetworkInfoEvent.h"

const bsc::NetworkInfo& bsc::NetworkInfoEvent::getNetworkInfo() const {
    return networkInfo;
}

void bsc::NetworkInfoEvent::setNetworkInfo(const NetworkInfo& networkInfo) {
    NetworkInfoEvent::networkInfo = networkInfo;
}
