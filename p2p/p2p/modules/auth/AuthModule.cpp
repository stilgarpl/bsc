//
// Created by stilgar on 19.07.18.
//

#include "AuthModule.h"

AuthModule::AuthModule(INode &node) : NodeModuleDependent(node) {}

void AuthModule::setupActions(LogicObject::SetupActionHelper &actionHelper) {

}

bool AuthModule::assignActions(LogicObject::AssignActionHelper &actionHelper) {
    return false;
}

bool AuthModule::setupSources(LogicObject::SetupSourceHelper &sourceHelper) {
    return false;
}
