//
// Created by stilgar on 23.08.17.
//

#include "ISource.h"
#include "SourceManager.h"

void SourceManager::addSource(std::shared_ptr<ISource> source) {
    lastProvider = source;
    sources.push_back(source);
//    source->setContext(commonContext);
    source->registerProviders(this);
}

void SourceManager::work() {
    ///@todo cos wymyslec, zebyt to nie zżerało 100% cpu
    for (auto &&i : sources) {
        if (i != nullptr) {
            i->work();
        }
    }
    std::this_thread::sleep_for(1ms);
}
