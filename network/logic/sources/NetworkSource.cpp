//
// Created by stilgar on 26.08.17.
//

#include "NetworkSource.h"
#include "../events/NetEvent.h"
#include "../../../logic/SourceManager.h"


void NetworkSource::work() {

}

void NetworkSource::registerProviders(SourceManager *manager) {
    manager->registerProvider<NetEvent>();
}
