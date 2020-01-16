//
// Created by stilgar on 27.10.18.
//

#include "ModuleSource.h"


bsc::ModuleSource::ModuleSource(bsc::SourceManager& sourceManager) : bsc::ISource(sourceManager),
                                                                     source(sourceManager) {

}

void bsc::ModuleSource::run() {
    waitForStop();
}

void bsc::ModuleSource::onStop() {
    source.stop();
    source.join();
}

void bsc::ModuleSource::onStart() {
    source.start();
}