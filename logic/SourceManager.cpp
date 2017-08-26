//
// Created by stilgar on 23.08.17.
//

#include "ISource.h"
#include "SourceManager.h"

void SourceManager::addSource(std::shared_ptr<ISource> source) {
    lastProvider = source;
    sources.push_back(source);
    source->registerProviders(this);
}
