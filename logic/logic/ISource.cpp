//
// Created by stilgar on 23.08.17.
//

#include "ISource.h"


bsc::ISource::ISource(SourceManager& sourceManager) : sourceManager(sourceManager) {}

//void ISource::registerProviders(SourceManager *sourceManager) {
////implement in derived classes as
//    // sourceManager->registerProvider<Tick>(this);
//    // sourceManager->registerProvider<Tick>(); // using last provider
//
//}
