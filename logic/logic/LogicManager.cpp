//
// Created by stilgar on 25.08.17.
//
#include "LogicManager.h"

void LogicManager::onStop() {
    LOGGER("LOGIC MANAGER IS STOPPING")
    sourceManager.stopSources();
    sourceManager.joinSources();
}

void LogicManager::onStart() {
    sourceManager.startSources();
}

void LogicManager::run() {
    LOGGER("logic stopping is " + std::to_string(this->isStopping()))
    waitForStop();

}

void LogicManager::setContexts(const Context::Ptr &context) {
    sourceManager.setContext(context);
}

LogicManager::~LogicManager() {
    LOGGER("logic destructor")

}
