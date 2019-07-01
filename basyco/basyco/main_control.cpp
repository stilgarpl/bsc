#include <iostream>
#include <thread>

//#define CEREAL_THREAD_SAFE 1
#include "p2p/node/Node.h"
#include "logic/sources/ClockSource.h"
#include "p2p/modules/nodeNetworkModule/protocol/logic/sources/ConnectionSource.h"
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

#include <p2p/modules/filesystem/FilesystemModule.h>
#include <p2p/dependency/Dependency.h>
#include <p2p/modules/command/CommandModule.h>
#include <p2p/modules/command/StandardCommandsDirectory.h>
#include <variant>
#include <p2p/modules/basic/BasicModule.h>


void setupModules(Node &node) {
    node.addModule<NodeNetworkModule>();
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
    cmd->mapRawCommand("remote", &CommandModule::sendRemoteCommand);
    cmd->mapRawCommand("broadcast", &CommandModule::broadcastRemoteCommand);
    cmd->mapCommand("shutdown", &BasicModule::shutdownNode);
    cmd->mapCommand("cd", &FilesystemModule::changeDirectory);
    cmd->mapCommand("pwd", &FilesystemModule::printWorkingDirectory);
    cmd->mapCommand("ls", &FilesystemModule::listCurrentDirectory);
//    cmd->mapCommand("scp", &FilesystemModule::remoteGetFile);
    cmd->mapCommand("scp", &FilesystemModule::remoteGetFile);
//    cmd->mapCommand("lscp", &FilesystemModule::printCurrentTransfers);

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


int main(int argc, char *argv[]) {

    if (argc <= 1) {
        return 0;
    }
    Node thisNode;

    //@todo load node info from file? or sth?
    thisNode.getNodeInfo().setNodeId("control");

    setupModules(thisNode);
    thisNode.getModule<NodeNetworkModule>()->addToNetwork("TheNetwork");
    thisNode.getModule<NodeNetworkModule>()->configuration().setPort(9191);

    auto cmdN = thisNode.getModule<CommandModule>();

    setupCommands(cmdN.get());
    cmdN->setInteractive(false);
    //@todo better way of running stuff from commandline
    std::vector<std::string> commands;
    for (int i = 1; i < argc; ++i) {
        commands.emplace_back(argv[i]);
    }



    thisNode.start();
    thisNode.waitUntilStarted();
//@todo get actual daemon address from configuration
    auto &daemonNode = thisNode.getModule<NodeNetworkModule>()->connectTo("127.0.0.1:9999");
    //@todo find a better way to wait for connection
    cmdN->sendCommandToRemoteNode(daemonNode, commands);
    thisNode.stop();

    thisNode.waitToFinish();


    return 0;
}
