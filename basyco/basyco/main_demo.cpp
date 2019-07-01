#include <iostream>
#include <thread>

//#define CEREAL_THREAD_SAFE 1
#include "p2p/node/Node.h"
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
#include <p2p/dependency/Dependency.h>
#include <p2p/modules/command/CommandModule.h>
#include <p2p/modules/command/StandardCommandsDirectory.h>
#include <variant>
#include <p2p/modules/basic/BasicModule.h>


void setupModules(Node &node) {
    node.addModule<BasicModule>();
    node.addModule<FilesystemModule>();
    node.addModule<network>();
    node.addModule<RepoModule>();
    node.addModule<CommandModule>();
}

void setupCommands(CommandModule *cmd) {
    cmd->mapCommand("t2", &CommandModule::testingMethodInt);
    cmd->submodule("tt").mapCommand("t2", &CommandModule::testingMethodInt);
    cmd->submodule("tt").submodule("xx").mapCommand("tx", &CommandModule::testingMethodInt);
    cmd->mapCommand("t3", &CommandModule::testingMethodIntFloat);
    cmd->mapCommand("connect", &network::connectTo);
    cmd->mapCommand("connectTo", &network::connectToNode);
    cmd->mapCommand<network, RemoteNode &, const NodeIdType &>("getnode", &network::getRemoteNode);
    cmd->mapCommand("disconnect", &network::disconnect);
    cmd->mapCommand("print", &network::printConnections);
//    cmd->mapCommand("update", &network:prin:updateNodeConnectionInfo);
//    cmd->mapCommand("purgeD", &network::purgeDuplicateConnections);
//    cmd->mapCommand("purgeI", &network::purgeInactiveConnections);
    cmd->mapRawCommand("remote", &CommandModule::sendRemoteCommand);
    cmd->mapRawCommand("broadcast", &CommandModule::broadcastRemoteCommand);
    cmd->mapCommand("shutdown", &BasicModule::shutdownNode);
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
    cmd->mapCommand("saveConf", &BasicModule::saveAllConfiguration);
    cmd->mapRawCommand("bg", &CommandModule::runInBackground);
    cmd->mapCommand("fireTrigV", &BasicModule::fireTriggerValue<std::string, std::string>);
    cmd->mapCommand("fireTrig", &BasicModule::fireTrigger<std::string>);
//    cmd->mapCommand("requestResource", &RepoModule::requestStoragePath);
//    cmd->mapCommand("beginTransfer", &FilesystemModule::beginTransferTest);
//    cmd->submodule("help").mapCommand("cmdList",&CommandModule::listCommands);

}


#include <spdlog/spdlog.h>


int main(int argc, char *argv[]) {

    spdlog::info("lala");

    Node thisNode;

    thisNode.getNodeInfo().setNodeId("first Node");

    setupModules(thisNode);
    thisNode.getModule<network>()->addToNetwork("TheNetwork");
    thisNode.getModule<network>()->configuration().setPort(9191);
    thisNode.addModule<AuthModule>();
    thisNode.getModule<AuthModule>()->getSubModule<AuthModule>().a = 5;
    thisNode.getModule<CommandModule>()->getSubModule<AuthModule>().a = 2;

    auto cmdN = thisNode.getModule<CommandModule>();

    setupCommands(cmdN.get());
    cmdN->setInteractive(true);

    thisNode.start();
//    setupProtocolLogic(thisNode.getLogicManager(), transmissionControl);

    Node otherNode;
    otherNode.getNodeInfo().setNodeId("second");


    setupModules(otherNode);
    otherNode.getModule<network>()->addToNetwork("TheNetwork");
    otherNode.getModule<network>()->configuration().setPort(9999);
    cmdN = otherNode.getModule<CommandModule>();
    setupCommands(cmdN.get());

    otherNode.start();
//    setupProtocolLogic(otherNode.getLogicManager(), transmissionControl);

    Node thirdNode;
    thirdNode.getNodeInfo().setNodeId("third");

    setupModules(thirdNode);
    thirdNode.getModule<network>()->addToNetwork("TheNetwork");
    thirdNode.getModule<network>()->configuration().setPort(9898);
    cmdN = thirdNode.getModule<CommandModule>();
    setupCommands(cmdN.get());
    cmdN = nullptr;
//    setupProtocolLogic(thirdNode.getLogicManager(), transmissionControl);
    thirdNode.start();

    //@todo this is required so logic is set up before connecting. change it so start wait until the end of the initialization phase
    // std::this_thread::sleep_for(1000ms);

    thisNode.getNodeInfo().printAll();
    otherNode.getNodeInfo().printAll();
    thirdNode.getNodeInfo().printAll();
    thisNode.getModule<network>()->connectTo("127.0.0.1:9999");
//    thisNode.getModule<network>()->connectTo("127.0.0.1:9898");
//    thisNode.getModule<network>()->updateNodeConnectionInfo();
//    otherNode.getModule<network>()->updateNodeConnectionInfo();
//    thirdNode.getModule<network>()->updateNodeConnectionInfo();


    thisNode.waitUntilStarted();
    otherNode.waitUntilStarted();

    //    thisNode.waitToFinish();
    //auto fdes = FileTransferControl::initiateTransfer(thisNode, "second", "/tmp/zsh", "/tmp/copied_zsh");
    thisNode.stop();
    thisNode.waitToFinish();
    otherNode.stop();
    otherNode.waitToFinish();
    thirdNode.stop();
    thirdNode.waitToFinish();


//    LOGGER("connection was ... " + std::to_string(ret));
//    otherNode.getModule<network>()->connectTo("127.0.0.1:9898");
    //  thisNode.getModule<network>()->connectTo("127.0.0.1:100");
//    std::this_thread::sleep_for(5s);

//    thisNode.getModule<network>()->updateNodeConnectionInfo();
//    otherNode.getModule<network>()->updateNodeConnectionInfo();
//    thirdNode.getModule<network>()->updateNodeConnectionInfo();
//
//    thisNode.getModule<network>()->printConnections();
//    otherNode.getModule<network>()->printConnections();
//    thirdNode.getModule<network>()->printConnections();
//
//    std::this_thread::sleep_for(1s);
//    thisNode.getModule<network>()->purgeDuplicateConnections();
//    otherNode.getModule<network>()->purgeDuplicateConnections();
//    thirdNode.getModule<network>()->purgeDuplicateConnections();
//    thisNode.getModule<network>()->purgeInactiveConnections();
//    otherNode.getModule<network>()->purgeInactiveConnections();
//    thirdNode.getModule<network>()->purgeInactiveConnections();
//    //LOGGER("lala");
//    std::this_thread::sleep_for(2s);
//    thisNode.getModule<network>()->printConnections();
//    otherNode.getModule<network>()->printConnections();
//    thirdNode.getModule<network>()->printConnections();
//    std::this_thread::sleep_for(9995s);
//    LOGGER("stopping");
//    thisNode.stop();
//    otherNode.stop();
//    thirdNode.stop();
//    std::this_thread::sleep_for(5s);
//    thisNode.getModule<network>()->purgeInactiveConnections();
//    otherNode.getModule<network>()->purgeInactiveConnections();
//    thirdNode.getModule<network>()->purgeInactiveConnections();
//    std::this_thread::sleep_for(45s);
//    thisNode.getModule<network>()->printConnections();
//    otherNode.getModule<network>()->printConnections();
//    thirdNode.getModule<network>()->printConnections();
//    LOGGER("finishing");
//
//    std::this_thread::sleep_for(115s);
//    LOGGER("finished");

    return 0;
}
