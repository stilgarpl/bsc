//
// Created by stilgar on 23.08.17.
//

#include "SourceManager.h"
#include "ISource.h"


ISource::ISource(SourceManager &sourceManager) : sourceManager(sourceManager) {}

//void ISource::registerProviders(SourceManager *sourceManager) {
////implement in derived classes as
//    // sourceManager->registerProvider<Tick>(this);
//    // sourceManager->registerProvider<Tick>(); // using last provider
//
//}
