//
// Created by stilgar on 23.08.17.
//

#include "SourceManager.h"


Context &ISource::getContext() {
    return context;
}

void ISource::setContext(const Context &context) {
    ISource::context += context;
}

//void ISource::registerProviders(SourceManager *sourceManager) {
////implement in derived classes as
//    // sourceManager->registerProvider<Tick>(this);
//    // sourceManager->registerProvider<Tick>(); // using last provider
//
//}
