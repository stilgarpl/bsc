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

IncorrectParametersException::IncorrectParametersException(size_t requiredParameters, size_t gotParameters)
        : requiredParameters(requiredParameters), gotParameters(gotParameters) {}
