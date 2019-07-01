//
// Created by stilgar on 06.10.17.
//

#include "NodeSource.h"
#include "NetworkSource.h"

void NetworkSource::networkInfoReceived(const NetworkInfo &networkInfo) {
    auto event = newEvent();
    event->setNetworkInfo(networkInfo);
    event->setEventId(EventType::IdType::NETWORK_INFO_RECEIVED);

    queueEvent(event);

}

NetworkSource::NetworkSource(SourceManager &sourceManager) : EventQueueSource(sourceManager) {}

void NetworkSource::saveNetworkInfo(const NetworkInfo &networkInfo) {
    auto event = newEvent();
    event->setNetworkInfo(networkInfo);
    event->setEventId(EventType::IdType::SAVE_NETWORK_INFO);

    queueEvent(event);
}
