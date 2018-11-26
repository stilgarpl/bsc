//
// Created by stilgar on 26.11.18.
//

#include "LogicStateSource.h"

void LogicStateSource::work() {
    source.work();

}

LogicStateSource::LogicStateSource(SourceManager &sourceManager) : ISource(sourceManager), source(sourceManager) {

}
