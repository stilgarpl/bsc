//
// Created by stilgar on 14.11.17.
//

#include "CommandModule.h"
#include "BasicModule.h"

CommandModule::CommandModule(INode &node) : NodeModule(node) {

    setRequired<BasicModule>();
}

void CommandModule::setupActions(LogicManager &logicManager) {

}

bool CommandModule::assignActions(LogicManager &logicManager) {
    return false;
}

bool CommandModule::setupSources(LogicManager &logicManager) {
    return false;
}
