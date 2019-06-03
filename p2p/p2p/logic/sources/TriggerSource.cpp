//
// Created by stilgar on 24.03.19.
//

#include "TriggerSource.h"


TriggerSource::TriggerSource(SourceManager &sourceManager) : ISource(sourceManager), source(sourceManager) {

}

void TriggerSource::run() {
    waitForStop();
}

void TriggerSource::onStop() {
    source.stop();
    source.join();
}

void TriggerSource::onStart() {
    source.start();
}