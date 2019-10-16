//
// Created by stilgar on 07.11.17.
//

#include <logic/sources/ClockSource.h>
#include <p2p/node/module/logic/ModuleSource.h>
#include "BasicModule.h"

BasicModule::BasicModule(INode &node) : NodeModuleDependent<BasicModule>(node, "basic") {

}

void BasicModule::setupActions(ILogicModule::SetupActionHelper &actionHelper) {


}

bool BasicModule::assignActions(ILogicModule::AssignActionHelper &actionHelper) {
    return true;
}

bool BasicModule::setupSources(ILogicModule::SetupSourceHelper &sourceHelper) {
    sourceHelper.requireSource<ClockSource>();
    sourceHelper.requireSource<ModuleSource>();
    return true;
}
