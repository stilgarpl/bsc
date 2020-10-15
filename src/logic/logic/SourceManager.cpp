//
// Created by Krzysztof Tulidowicz on 23.08.17.
//

#include "chain/GlobalChainContext.h"
#include "SourceManager.h"
#include "ISource.h"


namespace bsc {
    void SourceManager::addSource(const std::shared_ptr<bsc::ISource>& source) {
        std::lock_guard g(sourcesLock);
        sources.push_back(source);
//    source->setContext(commonContext);
    }


    void SourceManager::setContext(const Context::Ptr& context) {}

    void SourceManager::startSources() {
        std::lock_guard g(sourcesLock);
        for (auto&& source : sources) {
            source->start();
        }
    }

    void SourceManager::stopSources() {
        std::lock_guard g(sourcesLock);
        for (auto&& source : sources) {
            source->stop();
        }
    }

    void SourceManager::joinSources() {
        std::lock_guard g(sourcesLock);
        for (auto&& source : sources) {
            source->join();
        }
    }
    SourceManager::~SourceManager() {
        std::lock_guard g(sourcesLock);
        stopSources();
    }

    SourceManager::SourceManager() = default;

}