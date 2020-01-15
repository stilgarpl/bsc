//
// Created by stilgar on 14.11.17.
//

#include <p2p/modules/command/network/logic/actions/CommandActions.h>
#include <p2p/modules/command/network/packet/CommandPacket.h>
#include <core/io/InputOutputContext.h>

#include <utility>

#include "CommandModule.h"
#include "p2p/modules/basic/BasicModule.h"
#include "p2p/modules/network/NetworkModule.h"
#include "CommandInputOutputContext.h"

CommandModule::CommandModule(INode& node) : NodeModuleDependent(node, "command"),
                                            defaultCommandGroup(*this) {

    setRequired<BasicModule>();
}

void CommandModule::setupActions(ILogicModule::SetupActionHelper& actionHelper) {
//    actionHelper.setAction<CommandEvent>(CommandActions::RUN_COMMAND, CommandActions::runRemoteCommand);

    when(state<ILogicModule>(ModuleState::SUBMODULES_PREPARED).entered()).fireStateChangeReaction(
            [&](ILogicModule& module) {
                //@todo move this mechanism to NodeModule to auto collect all submodules from other modules.
                //  submodules probably have to be optional or sth.
                LOGGER("command submodule!")
                auto& commandSub = module.getSubModule<CommandModule>();
                commandSub.applyCommands(*this);
            });

}

bool CommandModule::assignActions(ILogicModule::AssignActionHelper& actionHelper) {
//    bool ret = actionHelper.assignAction<CommandEvent>(CommandEventId::EXECUTE_COMMAND, CommandActions::RUN_COMMAND);

//    when(event<CommandEvent>().withId(CommandEvent::IdType::EXECUTE_COMMAND)).fireNewAction([](auto e){ std::cout << "EXECUTING COMMAND!" << std::endl;});
//    when(event<CommandEvent>().withId(CommandEvent::IdType::DUMMY_COMMAND)).fireNewAction([](auto e){std::cout << "EXECUTING DUMMY INT COMMAND! "  << std::to_string(33)<< std::endl;});
//    CommandEvent dummyCommand;
//    dummyCommand.setEventId(CommandEvent::IdType::DUMMY_COMMAND);
//    when(event<CommandEvent>().withId(CommandEvent::IdType::EXECUTE_COMMAND)).emit(event<CommandEvent>().withId(CommandEvent::IdType::DUMMY_COMMAND));
//    when(event<CommandEvent>().withId(CommandEvent::IdType::EXECUTE_COMMAND)).fireNewAction(CommandActions::runRemoteCommand);
    return true;
//    return ret;
}

bool CommandModule::setupSources(ILogicModule::SetupSourceHelper& sourceHelper) {
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

}

void CommandModule::sendRemoteCommand(ArgumentContainerTypeRef args) {
    LOGGER("send remote")
    //@todo add checking if the number of parameters is correct
    if (args.size() >= 2) {
        std::string nodeId = args[0];
        std::string command = args[1];
        std::vector<std::string> rest(args.begin() + 2, args.end());

        auto netModule = node.getModule<NetworkModule>();
        CommandPacket::Request::Ptr packet = CommandPacket::Request::getNew();
        //CommandPacket::Request* packet;
        packet->setData(rest);
        packet->setCommandName(command);
        LOGGER("sending packet ")
        auto res = netModule->sendPacketToNode(nodeId, packet);

        if (res && res->isRunStatus()) {
            LOGGER("remote run successful")
            auto& out = Context::getActiveContext()->get<InputOutputContext>().out();
//            out << "Remote run result: \n --- \n" << res->getOutput() << std::endl << " --- \n";
            out << res->getOutput();
        } else {
            LOGGER("remote run failure")
        }


    }
}

void CommandModule::sendCommandToRemoteNode(RemoteNode& remoteNode, ArgumentContainerTypeRef args) {
    //@todo unify this and sendRemoteCommand
    LOGGER("send remote")
    //@todo add checking if the number of parameters is correct
    if (args.size() >= 1) {
        std::string command = args[0];
        std::vector<std::string> rest(args.begin() + 1, args.end());

        auto netModule = node.getModule<NetworkModule>();
        CommandPacket::Request::Ptr packet = CommandPacket::Request::getNew();
        //CommandPacket::Request* packet;
        packet->setData(rest);
        packet->setCommandName(command);
        LOGGER("sending packet ")
        auto res = remoteNode.sendRequestToNode(packet);

        if (res && res->isRunStatus()) {
            LOGGER("remote run successful")
            auto& out = Context::getActiveContext()->get<InputOutputContext>().out();
//            out << "Remote run result: \n --- \n" << res->getOutput() << std::endl << " --- \n";
            out << res->getOutput();
        } else {
            LOGGER("remote run failure")
        }


    }
}


void CommandModule::broadcastRemoteCommand(const std::vector<std::string>& args) {
    LOGGER("send broadcast")
    //@todo add checking if the number of parameters is correct
    if (args.size() >= 1) {
        std::string command = args[0];
        std::vector<std::string> rest(args.begin() + 1, args.end());

        auto netModule = node.getModule<NetworkModule>();
        CommandPacket::Request::Ptr packet = CommandPacket::Request::getNew();
        //CommandPacket::Request* packet;
        packet->setData(rest);
        packet->setCommandName(command);
        LOGGER("sending packet ")
        auto res = netModule->broadcastRequest(packet);

        for (const auto &[nodeId, response] : res) {
            LOGGER("broadcast received from " + nodeId)
            if (response && response->isRunStatus()) {
                LOGGER("remote run successful")
            } else {
                LOGGER("remote run failure")
            }
        }


    }
}

void CommandModule::runInBackground(const std::vector<std::string>& args) {

    if (args.size() >= 1) {
        std::string command = args[0];
        std::vector<std::string> rest(args.begin() + 1, args.end());
        auto activeContext = Context::getActiveContext();
        //@todo think about mutexes and thread safety
        std::thread([this, command, rest, activeContext] {
            Context::setActiveContext(activeContext);
            this->runCommand(command, rest);
        }).detach();


    }
}

void CommandModule::prepareSubmodules() {
    auto& networkSub = getSubModule<NetworkModule>();

    networkSub.registerPacketProcessor<CommandPacket>([this](const CommandPacket::Request::Ptr& request) {
        LOGGER("remote command!")

        //setting up remote command context.
        //@todo simplify context making
        Context::OwnPtr remoteCommandContext = Context::makeContext(Context::getActiveContext());
        auto ioContext = std::make_shared<CommandInputOutputContext>();
        remoteCommandContext->setDirect<InputOutputContext>(ioContext);
        {
            SetLocalContext localContext(remoteCommandContext);
            //@todo better return status handling
            bool runStatus =
                    this->runCommand(request->getCommandName(), request->getData()) == CommandExecutionStatus::success;
            CommandPacket::Response::Ptr res = CommandPacket::Response::getNew();
            res->setRunStatus(runStatus);
            res->setOutput(ioContext->getOutputStream().str());
            return res;
        }
    });

}

void CommandModule::parametersTestingCommand(const CommandModule::CommandPP& params) {
    auto& io = Context::getActiveContext()->get<InputOutputContext>();

    io.out() << std::string("got params " + std::to_string(params.a().value_or(-1)));
}


CommandModule::CommandGroup& CommandModule::CommandGroup::group(std::string name) {
    if (groups.count(name) == 0) {
        auto newGroup = std::make_shared<CommandGroup>(parent);
        groups[name] = newGroup;
        newGroup->groupHandler = parent.defaultGroupHandler;

    }
    return *groups[name];
}

CommandModule::SubModule::CommandData::CommandData(std::string commandName) : commandName(std::move(commandName)) {}

const std::string& CommandModule::SubModule::CommandData::getCommandName() const {
    return commandName;
}
