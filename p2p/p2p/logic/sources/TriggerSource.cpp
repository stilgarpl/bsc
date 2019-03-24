//
// Created by stilgar on 24.03.19.
//

#include "TriggerSource.h"

void TriggerSource::work() {
    source.work();

}

TriggerSource::TriggerSource(SourceManager &sourceManager) : ISource(sourceManager), source(sourceManager) {

}
