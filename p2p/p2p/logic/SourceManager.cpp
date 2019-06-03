//
// Created by stilgar on 23.08.17.
//

#include <p2p/logic/chain/GlobalChainContext.h>
#include "ISource.h"
#include "SourceManager.h"

void SourceManager::addSource(const std::shared_ptr<ISource> &source) {
    sources.push_back(source);
//    source->setContext(commonContext);
}


void SourceManager::setContext(const Context::Ptr &context) {
//    *commonContext += context;
    commonContext->setParentContext(context);
    commonContext->setDebug_id("source manager common context");
    //@todo there may be another way of initializing global chain context, but it may just as well be here...
//    commonContext->setDebug_id("common context source");
    context->set<GlobalChainContext>();
}

void SourceManager::startSources() {
    Context::setActiveContext(commonContext);
    for (auto &&source : sources) {
        source->start();
    }
}

void SourceManager::stopSources() {
    for (auto &&source : sources) {
        source->stop();
    }
}

void SourceManager::joinSources() {
    for (auto &&source : sources) {
        source->join();
    }
}

SourceManager::SourceManager() = default;
