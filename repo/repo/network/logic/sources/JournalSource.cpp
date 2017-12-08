//
// Created by stilgar on 18.10.17.
//

#include "JournalSource.h"

void JournalSource::work() {
    requestSource.work();
    responseSource.work();
}

void JournalSource::registerProviders(SourceManager *manager) {
    requestSource.registerProviders(manager);
    responseSource.registerProviders(manager);
    ///@todo context settings not here...
    requestSource.setContext(context);
    responseSource.setContext(context);
}
