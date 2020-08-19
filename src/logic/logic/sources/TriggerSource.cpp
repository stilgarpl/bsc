//
// Created by Krzysztof Tulidowicz on 24.03.19.
//

#include "TriggerSource.h"


bsc::TriggerSource::TriggerSource(SourceManager& sourceManager) : ISource(sourceManager), source(sourceManager) {

}

void bsc::TriggerSource::run() {
    waitForStop();
}

void bsc::TriggerSource::onStop() {
    source.stop();
    source.join();
}

void bsc::TriggerSource::onStart() {
    source.start();
}