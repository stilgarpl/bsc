#include <iostream>
#include <thread>

//#define CEREAL_THREAD_SAFE 1
#include "p2p/node/Node.h"
#include "p2p/logic/sources/ClockSource.h"
#include "p2p/modules/nodeNetworkModule/protocol/logic/sources/ConnectionSource.h"
#include "p2p/modules/nodeNetworkModule/protocol/logic/actions/TransmissionControl.h"
#include "p2p/modules/nodeNetworkModule/protocol/logic/actions/ProtocolActions.h"
#include "p2p/modules/nodeNetworkModule/protocol/logic/sources/NodeSource.h"
#include "p2p/modules/nodeNetworkModule/protocol/logic/actions/NodeActions.h"
#include "p2p/modules/nodeNetworkModule/protocol/logic/sources/NetworkSource.h"
#include "p2p/modules/nodeNetworkModule/protocol/logic/actions/NetworkActions.h"
#include <cereal/archives/xml.hpp>
#include <cereal/types/memory.hpp>
#include <repo/repository/storage/InternalStorage.h>
#include <p2p/modules/nodeNetworkModule/protocol/packet/NodeInfoGroup.h>
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
    node.addModule<NodeNetworkModule>();
    node.addModule<RepoModule>();
    node.addModule<CommandModule>();
}

void setupCommands(CommandModule *cmd) {
    cmd->mapCommand("t2", &CommandModule::testingMethodInt);
    cmd->submodule("tt").mapCommand("t2", &CommandModule::testingMethodInt);
    cmd->submodule("tt").submodule("xx").mapCommand("tx", &CommandModule::testingMethodInt);
    cmd->mapCommand("t3", &CommandModule::testingMethodIntFloat);
    cmd->mapCommand("connect", &NodeNetworkModule::connectTo);
    cmd->mapCommand("connectTo", &NodeNetworkModule::connectToNode);
    cmd->mapCommand<NodeNetworkModule, RemoteNode &, const NodeIdType &>("getnode", &NodeNetworkModule::getRemoteNode);
    cmd->mapCommand("disconnect", &NodeNetworkModule::disconnect);
    cmd->mapCommand("print", &NodeNetworkModule::printConnections);
//    cmd->mapCommand("update", &NodeNetworkModule:prin:updateNodeConnectionInfo);
//    cmd->mapCommand("purgeD", &NodeNetworkModule::purgeDuplicateConnections);
//    cmd->mapCommand("purgeI", &NodeNetworkModule::purgeInactiveConnections);
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
    cmd->mapCommand<RepoModule, void, const fs::path &>("persist", &RepoModule::persistFile);
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

    Node thisNode(9191);

    thisNode.getNodeInfo().setNodeId("first Node");

    setupModules(thisNode);
    thisNode.getModule<NodeNetworkModule>()->addToNetwork("TheNetwork");
    thisNode.getModule<NodeNetworkModule>()->configuration().setPort(9191);
    thisNode.addModule<AuthModule>();
    thisNode.getModule<AuthModule>()->getSubModule<AuthModule>().a = 5;
    thisNode.getModule<CommandModule>()->getSubModule<AuthModule>().a = 2;

    auto cmdN = thisNode.getModule<CommandModule>();

    setupCommands(cmdN.get());
    cmdN->setInteractive(true);

    thisNode.start();
//    setupProtocolLogic(thisNode.getLogicManager(), transmissionControl);

    Node otherNode(9999);
    otherNode.getNodeInfo().setNodeId("second");


    setupModules(otherNode);
    otherNode.getModule<NodeNetworkModule>()->addToNetwork("TheNetwork");
    otherNode.getModule<NodeNetworkModule>()->configuration().setPort(9999);
    cmdN = otherNode.getModule<CommandModule>();
    setupCommands(cmdN.get());

    otherNode.start();
//    setupProtocolLogic(otherNode.getLogicManager(), transmissionControl);

    Node thirdNode(9898);
    thirdNode.getNodeInfo().setNodeId("third");

    setupModules(thirdNode);
    thirdNode.getModule<NodeNetworkModule>()->addToNetwork("TheNetwork");
    thirdNode.getModule<NodeNetworkModule>()->configuration().setPort(9898);
    cmdN = thirdNode.getModule<CommandModule>();
    setupCommands(cmdN.get());
    cmdN = nullptr;
//    setupProtocolLogic(thirdNode.getLogicManager(), transmissionControl);
//    thirdNode.start();

    //@todo this is required so logic is set up before connecting. change it so start wait until the end of the initialization phase
    // std::this_thread::sleep_for(1000ms);

    thisNode.getNodeInfo().printAll();
    otherNode.getNodeInfo().printAll();
    thirdNode.getNodeInfo().printAll();
    thisNode.getModule<NodeNetworkModule>()->connectTo("127.0.0.1:9999");
//    thisNode.getModule<NodeNetworkModule>()->connectTo("127.0.0.1:9898");
//    thisNode.getModule<NodeNetworkModule>()->updateNodeConnectionInfo();
//    otherNode.getModule<NodeNetworkModule>()->updateNodeConnectionInfo();
//    thirdNode.getModule<NodeNetworkModule>()->updateNodeConnectionInfo();

    thisNode.waitToFinish();
    //auto fdes = FileTransferControl::initiateTransfer(thisNode, "second", "/tmp/zsh", "/tmp/copied_zsh");
    thisNode.stop();
    thisNode.waitToFinish();
    otherNode.stop();
    otherNode.waitToFinish();
    thirdNode.stop();
    thirdNode.waitToFinish();


//    LOGGER("connection was ... " + std::to_string(ret));
//    otherNode.getModule<NodeNetworkModule>()->connectTo("127.0.0.1:9898");
    //  thisNode.getModule<NodeNetworkModule>()->connectTo("127.0.0.1:100");
//    std::this_thread::sleep_for(5s);

//    thisNode.getModule<NodeNetworkModule>()->updateNodeConnectionInfo();
//    otherNode.getModule<NodeNetworkModule>()->updateNodeConnectionInfo();
//    thirdNode.getModule<NodeNetworkModule>()->updateNodeConnectionInfo();
//
//    thisNode.getModule<NodeNetworkModule>()->printConnections();
//    otherNode.getModule<NodeNetworkModule>()->printConnections();
//    thirdNode.getModule<NodeNetworkModule>()->printConnections();
//
//    std::this_thread::sleep_for(1s);
//    thisNode.getModule<NodeNetworkModule>()->purgeDuplicateConnections();
//    otherNode.getModule<NodeNetworkModule>()->purgeDuplicateConnections();
//    thirdNode.getModule<NodeNetworkModule>()->purgeDuplicateConnections();
//    thisNode.getModule<NodeNetworkModule>()->purgeInactiveConnections();
//    otherNode.getModule<NodeNetworkModule>()->purgeInactiveConnections();
//    thirdNode.getModule<NodeNetworkModule>()->purgeInactiveConnections();
//    //LOGGER("lala");
//    std::this_thread::sleep_for(2s);
//    thisNode.getModule<NodeNetworkModule>()->printConnections();
//    otherNode.getModule<NodeNetworkModule>()->printConnections();
//    thirdNode.getModule<NodeNetworkModule>()->printConnections();
//    std::this_thread::sleep_for(9995s);
//    LOGGER("stopping");
//    thisNode.stop();
//    otherNode.stop();
//    thirdNode.stop();
//    std::this_thread::sleep_for(5s);
//    thisNode.getModule<NodeNetworkModule>()->purgeInactiveConnections();
//    otherNode.getModule<NodeNetworkModule>()->purgeInactiveConnections();
//    thirdNode.getModule<NodeNetworkModule>()->purgeInactiveConnections();
//    std::this_thread::sleep_for(45s);
//    thisNode.getModule<NodeNetworkModule>()->printConnections();
//    otherNode.getModule<NodeNetworkModule>()->printConnections();
//    thirdNode.getModule<NodeNetworkModule>()->printConnections();
//    LOGGER("finishing");
//
//    std::this_thread::sleep_for(115s);
//    LOGGER("finished");

    return 0;
}
