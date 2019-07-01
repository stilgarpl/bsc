//
// Created by stilgar on 04.07.18.
//

#include "AutoSource.h"

AutoSource::AutoSource(SourceManager &sourceManager) : ISource(sourceManager) {}

void AutoSource::onStop() {
    //stop all threads.
    eventQueueSources.forEach<ISource>([](ISource &source) { source.stop(); });
    eventQueueSources.forEach<ISource>([](ISource &source) { source.join(); });
}

void AutoSource::run() {
    //all eventQueueSource threads will start automatically from queue executions.
    waitForStop();

}

