//
// Created by stilgar on 26.11.18.
//

#include "LogicStateSource.h"

LogicStateSource::LogicStateSource(SourceManager &sourceManager) : ISource(sourceManager), source(sourceManager) {

}

void LogicStateSource::run() {
    waitForStop();
}

void LogicStateSource::onStop() {
    source.stop();
    source.join();
}

void LogicStateSource::onStart() {
    source.start();
}

LogicStateSource::~LogicStateSource() = default;
