//
// Created by Krzysztof Tulidowicz on 04.07.18.
//

#include <logic/SourceManager.h>
#include "AutoSource.h"


namespace bsc {

    AutoSource::AutoSource(SourceManager& sourceManager) : ISource(sourceManager) {}

    void AutoSource::onStop() {
        std::unique_lock<std::mutex> g(sourcesLock);
        active = false;
        //stop all threads.
        eventQueueSources.forEach<ISource>([](ISource& source) { source.stop(); });
        eventQueueSources.forEach<ISource>([](ISource& source) { source.join(); });
    }

    void AutoSource::run() {
        //all eventQueueSource threads will start automatically from queue executions.
        waitForStop();

    }

}