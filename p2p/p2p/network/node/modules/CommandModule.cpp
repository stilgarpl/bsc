//
// Created by stilgar on 14.11.17.
//

#include <p2p/command/network/logic/actions/CommandActions.h>
#include <p2p/command/network/logic/sources/CommandSource.h>
#include <p2p/command/network/packet/CommandPacket.h>

#include "CommandModule.h"
#include "BasicModule.h"
#include "NodeNetworkModule.h"

CommandModule::CommandModule(INode &node) : NodeModule(node), defaultSubModule(*this) {

    setRequired<BasicModule>();
}

void CommandModule::setupActions(LogicManager &logicManager) {
    logicManager.setAction<CommandEvent>(CommandActions::RUN_COMMAND, CommandActions::runCommand);
}

bool CommandModule::assignActions(LogicManager &logicManager) {
    bool ret = logicManager.assignAction<CommandEvent>(CommandEventId::EXECUTE_COMMAND, CommandActions::RUN_COMMAND);
    return ret;
}

bool CommandModule::setupSources(LogicManager &logicManager) {
    logicManager.addSource<CommandSource>();
    return true;
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

void CommandModule::sendRemoteCommand(const std::vector<std::string> &args) {
    LOGGER("send remote")
    if (args.size() >= 2) {
        std::string nodeId = args[0];
        std::string command = args[1];
        std::vector<std::string> rest(args.begin() + 2, args.end());

        auto netModule = node.getModule<NodeNetworkModule>();
        CommandPacket::Request::Ptr packet = CommandPacket::Request::getNew();
        //CommandPacket::Request* packet;
        packet->setData(rest);
        packet->setCommandName(command);
        LOGGER("sending packet ")
        netModule->sendPacketToNode(nodeId, packet);


    }
}

CommandModule::CommandSubModule &CommandModule::CommandSubModule::submodule(std::string name) {
    if (submodules.count(name) == 0) {
        submodules[name] = std::make_shared<CommandSubModule>(parent);
    }

    return *submodules[name];
}
