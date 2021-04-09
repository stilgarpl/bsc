//
// Created by Krzysztof Tulidowicz on 25.08.17.
//
#include <log/log/Logger.h>
#include <logic/context/LogicContext.h>

void bsc::LogicManager::onStop() {
    logger.debug("LOGIC MANAGER IS STOPPING");
    sourceManager.stopSources();
    sourceManager.joinSources();
}

void bsc::LogicManager::onStart() {
    sourceManager.startSources();
}

void bsc::LogicManager::run() {
    logger.debug("logic stopping is " + std::to_string(this->isStopping()));
    waitForStop();
}

void bsc::LogicManager::setContexts(const Context::Ptr& context) {
    context->set<LogicContext, LogicManager&>(*this);
    sourceManager.setContext(context);
}

bsc::LogicManager::~LogicManager() {
    logger.debug("logic destructor");
    sourceManager.stopSources();
}
