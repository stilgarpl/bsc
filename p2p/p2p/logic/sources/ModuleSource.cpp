//
// Created by stilgar on 27.10.18.
//

#include "ModuleSource.h"

void ModuleSource::work() {
    source.work();
//    LOGGER("Module source work")

}

ModuleSource::ModuleSource(SourceManager &sourceManager) : ISource(sourceManager), source(sourceManager) {

}
