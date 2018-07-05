//
// Created by stilgar on 04.07.18.
//

#include "AutoSource.h"

AutoSource::AutoSource(SourceManager &sourceManager) : ISource(sourceManager) {}

void AutoSource::work() {

    eventQueueSources.forEach<ISource>([&](ISource &source) { source.work(); });

}
