//
// Created by stilgar on 19.07.18.
//

#include "AuthModule.h"

AuthModule::AuthModule(INode &node) : NodeModuleDependent<AuthModule>(node) {}

void AuthModule::setupActions(LogicObject::SetupActionHelper &actionHelper) {

}

bool AuthModule::assignActions(LogicObject::AssignActionHelper &actionHelper) {
    return false;
}

bool AuthModule::setupSources(LogicObject::SetupSourceHelper &sourceHelper) {
    return false;
}

void AuthModule::ready() {
    //@todo get auth submodules from all active modules.
}
