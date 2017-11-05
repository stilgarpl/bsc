//
// Created by stilgar on 05.11.17.
//

#include "RepoModule.h"

void RepoModule::setupActions(LogicManager &logicManager) {

}

bool RepoModule::assignActions(LogicManager &logicManager) {
    return false;
}

bool RepoModule::setupSources(LogicManager &logicManager) {
    return false;
}

RepoModule::RepoModule(INode &node) : NodeModule(node) {}
