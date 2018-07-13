//
// Created by stilgar on 14.11.17.
//

#include <p2p/modules/command/network/logic/actions/CommandActions.h>
#include <p2p/modules/command/network/logic/sources/CommandSource.h>
#include <p2p/modules/command/network/packet/CommandPacket.h>

#include "CommandModule.h"
#include "p2p/modules/basic/BasicModule.h"
#include "p2p/modules/nodeNetworkModule/NodeNetworkModule.h"

CommandModule::CommandModule(INode &node) : NodeModuleDependent(node), defaultSubModule(*this) {

    setRequired<BasicModule>();
}

void CommandModule::setupActions(ILogicModule::SetupActionHelper &actionHelper) {
    actionHelper.setAction<CommandEvent>(CommandActions::RUN_COMMAND, CommandActions::runCommand);
}

bool CommandModule::assignActions(ILogicModule::AssignActionHelper &actionHelper) {
//    bool ret = actionHelper.assignAction<CommandEvent>(CommandEventId::EXECUTE_COMMAND, CommandActions::RUN_COMMAND);
    when(event<CommandEvent>().withId(CommandEvent::IdType::EXECUTE_COMMAND)).fireAction(CommandActions::RUN_COMMAND);
//    when(event<CommandEvent>().withId(CommandEvent::IdType::EXECUTE_COMMAND)).fireNewAction([](auto e){ std::cout << "EXECUTING COMMAND!" << std::endl;});
//    when(event<CommandEvent>().withId(CommandEvent::IdType::DUMMY_COMMAND)).fireNewAction([](auto e){std::cout << "EXECUTING DUMMY INT COMMAND! "  << std::to_string(33)<< std::endl;});
//    CommandEvent dummyCommand;
//    dummyCommand.setEventId(CommandEvent::IdType::DUMMY_COMMAND);
//    when(event<CommandEvent>().withId(CommandEvent::IdType::EXECUTE_COMMAND)).emit(event<CommandEvent>().withId(CommandEvent::IdType::DUMMY_COMMAND));
//    when(event<CommandEvent>().withId(CommandEvent::IdType::EXECUTE_COMMAND)).fireNewAction(CommandActions::runCommand);
    return true;
//    return ret;
}

bool CommandModule::setupSources(ILogicModule::SetupSourceHelper &sourceHelper) {
    sourceHelper.requireSource<CommandSource>();
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
    ///@todo add checking if the number of parameters is correct
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
        auto res = netModule->sendPacketToNode(nodeId, packet);

        if (res && res->isRunStatus()) {
            LOGGER("remote run successful")
        } else {
            LOGGER("remote run failure")
        }


    }
}


CommandModule::CommandSubModule &CommandModule::CommandSubModule::submodule(std::string name) {
    if (submodules.count(name) == 0) {
        submodules[name] = std::make_shared<CommandSubModule>(parent);
    }

    return *submodules[name];
}
