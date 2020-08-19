//
// Created by Krzysztof Tulidowicz on 16.05.19.
//


#include "INodeModule.h"


void bsc::INodeModule::doInitialize() {
    initialize();
    changeModuleState(ModuleState::INITIALIZED);
}

void bsc::INodeModule::doSetupLogic() {
    if (setupLogic()) {
        changeModuleState(ModuleState::LOGIC_READY);
    }

}

void bsc::INodeModule::doReady() {
    ready();
    changeModuleState(ModuleState::READY);
}

void bsc::INodeModule::doShutdown() {
    shutdown();
    changeModuleState(ModuleState::SHUTDOWN);
}

void bsc::INodeModule::doPrepareSubmodules() {
    prepareSubmodules();
    changeModuleState(ModuleState::SUBMODULES_PREPARED);
}

void bsc::INodeModule::doSaveConfiguration() {
    saveConfiguration();

}
