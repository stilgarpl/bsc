//
// Created by Krzysztof Tulidowicz on 14.11.17.
//

#include <p2p/modules/command/network/logic/actions/CommandActions.h>
#include <p2p/modules/command/network/packet/CommandPacket.h>
#include <core/io/InputOutputContext.h>

#include <utility>

#include "CommandModule.h"
#include "p2p/modules/basic/BasicModule.h"
#include "p2p/modules/network/NetworkModule.h"
#include "CommandInputOutputContext.h"

namespace bsc {

    CommandModule::CommandModule(bsc::INode& node) : NodeModuleDependent(node, "command"),
                                                     defaultCommandGroup(*this) {

        setRequired<bsc::BasicModule>();
    }

    void CommandModule::setupActions(bsc::ILogicModule::SetupActionHelper& actionHelper) {
//    actionHelper.setAction<CommandEvent>(CommandActions::RUN_COMMAND, CommandActions::runRemoteCommand);

        when(state<ILogicModule>(bsc::ModuleState::SUBMODULES_PREPARED).entered()).fireStateChangeReaction(
                [&](ILogicModule& module) {
                    //@todo move this mechanism to NodeModule to auto collect all submodules from other modules.
                    //  submodules probably have to be optional or sth.
                    logger.debug("command submodule!");
                    auto& commandSub = module.getSubModule<CommandModule>();
                    commandSub.applyCommands(*this);
                });

    }

    bool CommandModule::assignActions(bsc::ILogicModule::AssignActionHelper& actionHelper) {
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

    bool CommandModule::setupSources(bsc::ILogicModule::SetupSourceHelper& sourceHelper) {
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

    void CommandModule::sendRemoteCommand(ArgumentContainerTypeCRef args) {
        logger.debug("send remote");
        //@todo add checking if the number of parameters is correct
        if (args.size() >= 2) {
            std::string nodeId  = args[0];
            std::string command = args[1];
            std::vector<std::string> rest(args.begin() + 2, args.end());

            auto netModule                          = node.getModule<bsc::NetworkModule>();
            bsc::CommandPacket::Request::Ptr packet = bsc::CommandPacket::Request::getNew();
            // CommandPacket::Request* packet;
            packet->setData(rest);
            packet->setCommandName(command);
            logger.debug("sending packet ");
            auto res = netModule->sendPacketToNode(nodeId, packet);

            if (res && res->isRunStatus()) {
                logger.debug("remote run successful");
                auto& out = Context::getActiveContext()->get<InputOutputContext>()->out();
//            out << "Remote run result: \n --- \n" << res->getOutput() << std::endl << " --- \n";
                out << res->getOutput();
            } else {
                logger.debug("remote run failure");
            }


        }
    }

    void CommandModule::sendCommandToRemoteNode(RemoteNode& remoteNode, ArgumentContainerTypeCRef args) {
        //@todo unify this and sendRemoteCommand
        logger.debug("send remote");
        //@todo add checking if the number of parameters is correct
        if (args.size() >= 1) {
            std::string command = args[0];
            std::vector<std::string> rest(args.begin() + 1, args.end());

            auto netModule                          = node.getModule<bsc::NetworkModule>();
            bsc::CommandPacket::Request::Ptr packet = bsc::CommandPacket::Request::getNew();
            // CommandPacket::Request* packet;
            packet->setData(rest);
            packet->setCommandName(command);
            logger.debug("sending packet ");
            auto res = remoteNode.sendRequestToNode(packet);

            if (res && res->isRunStatus()) {
                logger.debug("remote run successful");
                auto& out = Context::getActiveContext()->get<InputOutputContext>()->out();
//            out << "Remote run result: \n --- \n" << res->getOutput() << std::endl << " --- \n";
                out << res->getOutput();
            } else {
                logger.debug("remote run failure");
            }


        }
    }


    void CommandModule::broadcastRemoteCommand(const std::vector<std::string>& args) {
        logger.debug("send broadcast");
        //@todo add checking if the number of parameters is correct
        if (args.size() >= 1) {
            std::string command = args[0];
            std::vector<std::string> rest(args.begin() + 1, args.end());

            auto netModule = node.getModule<bsc::NetworkModule>();
            bsc::CommandPacket::Request::Ptr packet = bsc::CommandPacket::Request::getNew();
            //CommandPacket::Request* packet;
            packet->setData(rest);
            packet->setCommandName(command);
            logger.debug("sending packet ");
            auto res = netModule->broadcastRequest(packet);

            for (const auto &[nodeId, response] : res) {
                logger.debug("broadcast received from " + nodeId);
                if (response && response->isRunStatus()) {
                    logger.debug("remote run successful");
                } else {
                    logger.debug("remote run failure");
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
        auto& networkSub = getSubModule<bsc::NetworkModule>();

        networkSub.registerPacketProcessor<bsc::CommandPacket>([this](const bsc::CommandPacket::Request::Ptr& request) {
            logger.debug("remote command!");

            //setting up remote command context.
            //@todo simplify context making
            Context::OwnPtr remoteCommandContext = Context::makeContext(Context::getActiveContext());
            auto ioContext = std::make_shared<bsc::CommandInputOutputContext>();
            remoteCommandContext->setDirect<bsc::InputOutputContext>(ioContext);
            {
                bsc::SetLocalContext localContext(remoteCommandContext);
                //@todo better return status handling
                bool runStatus =
                        this->runCommand(request->getCommandName(), request->getData()) ==
                        CommandExecutionStatus::success;
                bsc::CommandPacket::Response::Ptr res = bsc::CommandPacket::Response::getNew();
                res->setRunStatus(runStatus);
                res->setOutput(ioContext->getOutputStream().str());
                return res;
            }
        });

    }

    void CommandModule::parametersTestingCommand(const CommandModule::CommandPP& params) {
        auto io = Context::getActiveContext()->get<bsc::InputOutputContext>();

        io->out() << std::string("got params " + std::to_string(params.a().value_or(-1)));
    }
    void CommandModule::run() {
        //@todo restart this if flag changes later?
        if (interactive) {
            runInteractive();
        }
        waitForStop();
    }
    void CommandModule::runInteractive() {
        std::string line;
        node.setNodeContextActive();
        while (!isStopping() && std::getline(std::cin, line)) {
            runLine(line);


        }

        logger.debug("Command Module has stopped");
    }
    void CommandModule::runLine(const std::string& line) {
        logger.debug("Command: " + line);
        //explode command into words
        auto words = bsc::explode(line, ' ');

        std::string groupOrCommandName = "";
        //  std::string commandName = "";
        std::vector<std::string> data;
        if (!words.empty()) groupOrCommandName = words[0];
        //if (words.size() > 1) commandName = words[1];
        if (words.size() > 1) {
            auto b = words.begin() + 1;
            auto e = words.end();
            data.insert(data.end(), b, e);
        }

        try {
            runCommand(groupOrCommandName, data);
        } catch (const bsc::IncorrectParametersCountException& e) {
            logger.debug("Incorrect parameters. Required: " + std::to_string(e.requiredParameters) +
                         " got: " + std::to_string(e.gotParameters));
        }
    }


    CommandModule::CommandGroup& CommandModule::CommandGroup::group(const std::string& name) {
        if (groups.count(name) == 0) {
            auto newGroup = std::make_shared<CommandGroup>(parent);
            groups[name] = newGroup;
            newGroup->groupHandler = parent.defaultGroupHandler;

        }
        return *groups[name];
    }

    CommandModule::SubModule::CommandData::CommandData(std::string commandName) : commandName(std::move(commandName)) {}

    const std::string& CommandModule::SubModule::CommandData::getCommandName() const { return commandName; }
    InvalidCommandException::InvalidCommandException(const std::string& arg) : domain_error(arg) {}
}