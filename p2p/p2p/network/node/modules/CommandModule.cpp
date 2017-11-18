//
// Created by stilgar on 14.11.17.
//

#include "CommandModule.h"
#include "BasicModule.h"

CommandModule::CommandModule(INode &node) : NodeModule(node), defaultSubModule(*this) {

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

bool CommandModule::isInteractive() const {
    return interactive;
}

void CommandModule::setInteractive(bool interactive) {
    CommandModule::interactive = interactive;
}

void CommandModule::initialize() {
    NodeModule::initialize();

    for (auto &&directory : commandsDirectory) {
        directory->configureCommands(*this);
    }
}

CommandModule::CommandSubModule &CommandModule::CommandSubModule::submodule(std::string name) {
    if (submodules.count(name) == 0) {
        submodules[name] = std::make_shared<CommandSubModule>(parent);
    }

    return *submodules[name];
}
