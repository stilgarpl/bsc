//
// Created by stilgar on 16.05.19.
//


#include "INodeModule.h"

void INodeModule::doInitialize() {
    initialize();
    changeModuleState(ModuleState::INITIALIZED);
}

void INodeModule::doSetupLogic() {
    if (setupLogic()) {
        changeModuleState(ModuleState::LOGIC_READY);
    }

}

void INodeModule::doReady() {
    ready();
    changeModuleState(ModuleState::READY);
}

void INodeModule::doShutdown() {
    shutdown();
    changeModuleState(ModuleState::SHUTDOWN);
}

void INodeModule::doPrepareSubmodules() {
    prepareSubmodules();
    changeModuleState(ModuleState::SUBMODULES_PREPARED);
}

void INodeModule::doSaveConfiguration() {
    saveConfiguration();

}
