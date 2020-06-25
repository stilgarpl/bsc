//
// Created by stilgar on 25.08.17.
//
#include <logic/context/LogicContext.h>


void bsc::LogicManager::onStop() {
    LOGGER("LOGIC MANAGER IS STOPPING")
    sourceManager.stopSources();
    sourceManager.joinSources();
}

void bsc::LogicManager::onStart() {
    sourceManager.startSources();
}

void bsc::LogicManager::run() {
    LOGGER("logic stopping is " + std::to_string(this->isStopping()))
    waitForStop();

}

void bsc::LogicManager::setContexts(const Context::Ptr& context) {
    context->set<bsc::LogicContext, LogicManager&>(*this);
    sourceManager.setContext(context);
}

bsc::LogicManager::~LogicManager() {
    LOGGER("logic destructor")
    sourceManager.stopSources();

}
