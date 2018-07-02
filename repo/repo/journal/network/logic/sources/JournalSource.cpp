//
// Created by stilgar on 18.10.17.
//

#include "JournalSource.h"

void JournalSource::work() {
    requestSource.work();
    responseSource.work();
}


JournalSource::JournalSource(SourceManager &sourceManager) : ISource(sourceManager), requestSource(sourceManager),
                                                             responseSource(sourceManager) {}
