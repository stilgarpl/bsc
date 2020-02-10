#include <iostream>
#include <thread>

//#define CEREAL_THREAD_SAFE 1
#include "p2p/core/node/Node.h"
#include "logic/sources/ClockSource.h"
#include "p2p/modules/network/protocol/logic/sources/ConnectionSource.h"
#include "p2p/modules/network/protocol/logic/actions/ProtocolActions.h"
#include "p2p/modules/network/protocol/logic/sources/NodeSource.h"
#include "p2p/modules/network/protocol/logic/actions/NodeActions.h"
#include "p2p/modules/network/protocol/logic/sources/NetworkSource.h"
#include "p2p/modules/network/protocol/logic/actions/NetworkActions.h"
#include <cereal/archives/xml.hpp>
#include <cereal/types/memory.hpp>
#include <repo/repository/storage/InternalStorage.h>
#include <p2p/modules/network/protocol/packet/NodeInfoGroup.h>
#include <p2p/modules/auth/AuthModule.h>


using namespace std::chrono_literals;

#include <repo/journal/SimpleJournal.h>
#include <repo/node/RepoModule.h>
#include <p2p/modules/filesystem/FilesystemModule.h>
#include <p2p/modules/command/CommandModule.h>
#include <p2p/modules/basic/BasicModule.h>
#include <p2p/modules/command/DefaultCommandGroupParameters.h>
using namespace bsc;

void setupModules(bsc::Node& node) {
    node.addModule<bsc::BasicModule>();
    node.addModule<FilesystemModule>();
    node.addModule<bsc::NetworkModule>();
    node.addModule<RepoModule>();
    node.addModule<CommandModule>();
}

void setupCommands(CommandModule *cmd) {
    cmd->mapCommand("t2", &CommandModule::testingMethodInt);
    cmd->setDefaultGroupHandler<bsc::DefaultCommandGroupParameters>(defaultCommandGroupHandler);
    cmd->group("tt").mapCommand("t2", &CommandModule::testingMethodInt);
    cmd->group("tt").group("xx").mapCommand("tx", &CommandModule::testingMethodInt);
//    cmd->group("tt").handler<CommandModule::CommandPP>([](auto properties) {
//        LOGGER("handler" + std::to_string(properties.a().value_or(-1)));
//        return CommandModule::CommandExecutionStatus::success;
//    });

    cmd->mapCommand("t3", &CommandModule::testingMethodIntFloat);
    cmd->mapCommand("connect", &bsc::NetworkModule::connectTo);
    cmd->mapCommand("connectTo", &bsc::NetworkModule::connectToNode);
    cmd->mapCommand<bsc::NetworkModule, RemoteNode&, const NodeIdType&>("getnode", &bsc::NetworkModule::getRemoteNode);
    cmd->mapCommand("disconnect", &bsc::NetworkModule::disconnect);
    cmd->mapCommand("print", &bsc::NetworkModule::printConnections);
//    cmd->mapCommand("update", &network:prin:updateNodeConnectionInfo);
//    cmd->mapCommand("purgeD", &NetworkModule::purgeDuplicateConnections);
//    cmd->mapCommand("purgeI", &NetworkModule::purgeInactiveConnections);
    cmd->mapRawCommand("remote", &CommandModule::sendRemoteCommand);
    cmd->mapRawCommand("broadcast", &CommandModule::broadcastRemoteCommand);
    cmd->mapCommand("shutdown", &bsc::BasicModule::shutdownNode);
    cmd->mapCommand("cd", &FilesystemModule::changeDirectory);
    cmd->mapCommand("pwd", &FilesystemModule::printWorkingDirectory);
    cmd->mapCommand("ls", &FilesystemModule::listCurrentDirectory);
//    cmd->mapCommand("scp", &FilesystemModule::remoteGetFile);
    cmd->mapCommand("scp", &FilesystemModule::remoteGetFile);
//    cmd->mapCommand("lscp", &FilesystemModule::printCurrentTransfers);

    cmd->mapCommand("createRep", &RepoModule::createRepository);
    cmd->mapCommand("selectRep", &RepoModule::selectRepository);
    cmd->mapCommand("saveRep", &RepoModule::saveRepository);
    cmd->mapCommand("loadRep", &RepoModule::loadRepository);
    cmd->mapCommand("printRep", &RepoModule::printHistory);
    cmd->mapCommand("downRep", &RepoModule::downloadRemoteRepository);
    cmd->mapCommand("resRep", &RepoModule::restoreRepository);
    cmd->mapCommand("update", &RepoModule::updateFile);
    cmd->mapCommand("ignore", &RepoModule::ignoreFile);
    cmd->mapCommand("updateAll", &RepoModule::updateAllFiles);
    cmd->mapCommand("deploy", &RepoModule::deployAllFiles);
    cmd->mapCommand("run", &CommandModule::runScript);
    cmd->mapCommand("sleep", &CommandModule::sleep);
    cmd->mapCommand("saveConf", &bsc::BasicModule::saveAllConfiguration);
    cmd->mapRawCommand("bg", &CommandModule::runInBackground);
    cmd->mapCommand("fireTrigV", &bsc::BasicModule::fireTriggerValue<std::string, std::string>);
    cmd->mapCommand("fireTrig", &bsc::BasicModule::fireTrigger<std::string>);
    cmd->mapCommand("aaa", &CommandModule::parametersTestingCommand, CommandModule::CommandPP{});
//    cmd->mapCommand("requestResource", &RepoModule::requestStoragePath);
//    cmd->mapCommand("beginTransfer", &FilesystemModule::beginTransferTest);
//    cmd->submodule("help").mapCommand("cmdList",&CommandModule::listCommands);

}


#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"



int main(int argc, char* argv[]) {

    auto file_logger = spdlog::basic_logger_mt("basic_logger", "/tmp/bsc.log");
    spdlog::set_default_logger(file_logger);
    spdlog::flush_every(std::chrono::seconds(5));

    spdlog::info("lala");

    bsc::Node thisNode;

    thisNode.getNodeInfo().setNodeId("first Node");

    setupModules(thisNode);
    thisNode.getModule<bsc::NetworkModule>()->addToNetwork("TheNetwork");
    thisNode.getModule<bsc::NetworkModule>()->configuration().setPort(9191);
    thisNode.addModule<bsc::AuthModule>();
    thisNode.getModule<bsc::AuthModule>()->getSubModule<bsc::AuthModule>().a = 5;
    thisNode.getModule<CommandModule>()->getSubModule<bsc::AuthModule>().a = 2;

    auto cmdN = thisNode.getModule<CommandModule>();

    setupCommands(cmdN.get());
    cmdN->setInteractive(true);

    thisNode.start();
//    setupProtocolLogic(thisNode.getLogicManager(), transmissionControl);

    bsc::Node otherNode;
    otherNode.getNodeInfo().setNodeId("second");


    setupModules(otherNode);
    otherNode.getModule<bsc::NetworkModule>()->addToNetwork("TheNetwork");
    otherNode.getModule<bsc::NetworkModule>()->configuration().setPort(9999);
    cmdN = otherNode.getModule<CommandModule>();
    setupCommands(cmdN.get());

    otherNode.start();
//    setupProtocolLogic(otherNode.getLogicManager(), transmissionControl);

    bsc::Node thirdNode;
    thirdNode.getNodeInfo().setNodeId("third");

    setupModules(thirdNode);
    thirdNode.getModule<bsc::NetworkModule>()->addToNetwork("TheNetwork");
    thirdNode.getModule<bsc::NetworkModule>()->configuration().setPort(9898);
    cmdN = thirdNode.getModule<CommandModule>();
    setupCommands(cmdN.get());
    cmdN = nullptr;
//    setupProtocolLogic(thirdNode.getLogicManager(), transmissionControl);
    thirdNode.start();

    thisNode.waitUntilStarted();
    otherNode.waitUntilStarted();
    thirdNode.waitUntilStarted();
    thisNode.getModule<bsc::NetworkModule>()->connectTo("127.0.0.1:9999");
    //    thisNode.waitToFinish();
    //auto fdes = FileTransferControl::initiateTransfer(thisNode, "second", "/tmp/zsh", "/tmp/copied_zsh");
//    thisNode.stop();
    thisNode.waitToFinish();
    otherNode.stop();
    otherNode.waitToFinish();
    thirdNode.stop();
    thirdNode.waitToFinish();


//    LOGGER("connection was ... " + std::to_string(ret));
//    otherNode.getModule<NetworkModule>()->connectTo("127.0.0.1:9898");
    //  thisNode.getModule<NetworkModule>()->connectTo("127.0.0.1:100");
//    std::this_thread::sleep_for(5s);

//    thisNode.getModule<NetworkModule>()->updateNodeConnectionInfo();
//    otherNode.getModule<NetworkModule>()->updateNodeConnectionInfo();
//    thirdNode.getModule<NetworkModule>()->updateNodeConnectionInfo();
//
//    thisNode.getModule<NetworkModule>()->printConnections();
//    otherNode.getModule<NetworkModule>()->printConnections();
//    thirdNode.getModule<NetworkModule>()->printConnections();
//
//    std::this_thread::sleep_for(1s);
//    thisNode.getModule<NetworkModule>()->purgeDuplicateConnections();
//    otherNode.getModule<NetworkModule>()->purgeDuplicateConnections();
//    thirdNode.getModule<NetworkModule>()->purgeDuplicateConnections();
//    thisNode.getModule<NetworkModule>()->purgeInactiveConnections();
//    otherNode.getModule<NetworkModule>()->purgeInactiveConnections();
//    thirdNode.getModule<NetworkModule>()->purgeInactiveConnections();
//    //LOGGER("lala");
//    std::this_thread::sleep_for(2s);
//    thisNode.getModule<NetworkModule>()->printConnections();
//    otherNode.getModule<NetworkModule>()->printConnections();
//    thirdNode.getModule<NetworkModule>()->printConnections();
//    std::this_thread::sleep_for(9995s);
//    LOGGER("stopping");
//    thisNode.stop();
//    otherNode.stop();
//    thirdNode.stop();
//    std::this_thread::sleep_for(5s);
//    thisNode.getModule<NetworkModule>()->purgeInactiveConnections();
//    otherNode.getModule<NetworkModule>()->purgeInactiveConnections();
//    thirdNode.getModule<NetworkModule>()->purgeInactiveConnections();
//    std::this_thread::sleep_for(45s);
//    thisNode.getModule<NetworkModule>()->printConnections();
//    otherNode.getModule<NetworkModule>()->printConnections();
//    thirdNode.getModule<NetworkModule>()->printConnections();
//    LOGGER("finishing");
//
//    std::this_thread::sleep_for(115s);
//    LOGGER("finished");

    return 0;
}