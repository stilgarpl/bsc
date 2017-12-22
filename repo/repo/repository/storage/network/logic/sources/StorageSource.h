//
// Created by stilgar on 22.12.17.
//

#ifndef BASYCO_STORAGESOURCE_H
#define BASYCO_STORAGESOURCE_H


#include <p2p/logic/sources/EventQueueSource.h>
#include <repo/repository/storage/network/logic/events/StorageEvent.h>

class StorageSource : public EventQueueSource<StorageEvent> {

};


#endif //BASYCO_STORAGESOURCE_H
