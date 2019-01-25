//
// Created by stilgar on 07.11.17.
//

#include <p2p/logic/sources/ClockSource.h>
#include <p2p/modules/nodeNetworkModule/protocol/logic/actions/NodeActions.h>
#include <p2p/logic/sources/ModuleSource.h>
#include "BasicModule.h"

BasicModule::BasicModule(INode &node) : NodeModuleDependent<BasicModule>(node) {

}

void BasicModule::setupActions(ILogicModule::SetupActionHelper &actionHelper) {


}

bool BasicModule::assignActions(ILogicModule::AssignActionHelper &actionHelper) {
    return false;
}

bool BasicModule::setupSources(ILogicModule::SetupSourceHelper &sourceHelper) {
    sourceHelper.requireSource<ClockSource>();
    sourceHelper.requireSource<ModuleSource>();
    return true;
}
