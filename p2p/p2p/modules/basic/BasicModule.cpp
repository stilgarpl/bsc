//
// Created by stilgar on 07.11.17.
//

#include <logic/sources/ClockSource.h>
#include <p2p/core/node/module/logic/ModuleSource.h>
#include "BasicModule.h"


bsc::BasicModule::BasicModule(bsc::INode& node) : NodeModuleDependent<BasicModule>(node, "basic") {

}

void bsc::BasicModule::setupActions(bsc::ILogicModule::SetupActionHelper& actionHelper) {


}

bool bsc::BasicModule::assignActions(bsc::ILogicModule::AssignActionHelper& actionHelper) {
    return true;
}

bool bsc::BasicModule::setupSources(bsc::ILogicModule::SetupSourceHelper& sourceHelper) {
    sourceHelper.requireSource<bsc::ClockSource>();
    sourceHelper.requireSource<bsc::ModuleSource>();
    return true;
}
