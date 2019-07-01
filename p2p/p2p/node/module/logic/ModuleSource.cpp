//
// Created by stilgar on 27.10.18.
//

#include "ModuleSource.h"


ModuleSource::ModuleSource(SourceManager &sourceManager) : ISource(sourceManager), source(sourceManager) {

}

void ModuleSource::run() {
    waitForStop();
}

void ModuleSource::onStop() {
    source.stop();
    source.join();
}

void ModuleSource::onStart() {
    source.start();
}