
#include "p2p/core/node/Node.h"
#include "p2p/modules/network/protocol/logic/sources/NodeSource.h"
#include "p2p/modules/network/protocol/logic/actions/NodeActions.h"
#include "p2p/modules/network/protocol/logic/actions/NetworkActions.h"
#include <repo/node/RepoModule.h>
#include <p2p/modules/filesystem/FilesystemModule.h>
#include <p2p/modules/command/CommandModule.h>
#include <p2p/modules/basic/BasicModule.h>
#include <Poco/Environment.h>

using namespace std::chrono_literals;
using namespace bsc;

void setupModules(Node& node) {
    node.addModule<BasicModule>();
    node.addModule<FilesystemModule>();
    node.addModule<NetworkModule>();
    node.addModule<RepoModule>();
    node.addModule<CommandModule>();
}

//@todo commands should be setDirect up by submodule, right now they are redeclared in every main
void setupCommands(CommandModule *cmd) {
    cmd->mapCommand("t2", &CommandModule::testingMethodInt);
    cmd->group("tt").mapCommand("t2", &CommandModule::testingMethodInt);
    cmd->group("tt").group("xx").mapCommand("tx", &CommandModule::testingMethodInt);
    cmd->mapCommand("t3", &CommandModule::testingMethodIntFloat);
    cmd->mapCommand("connect", &NetworkModule::connectTo);
    cmd->mapCommand("connectTo", &NetworkModule::connectToNode);
    cmd->mapCommand<NetworkModule, RemoteNode&, const NodeIdType&>("getnode", &NetworkModule::getRemoteNode);
    cmd->mapCommand("disconnect", &NetworkModule::disconnect);
    cmd->mapCommand("print", &NetworkModule::printConnections);
    cmd->mapRawCommand("remote", &CommandModule::sendRemoteCommand);
    cmd->mapRawCommand("broadcast", &CommandModule::broadcastRemoteCommand);
    cmd->mapCommand("shutdown", &BasicModule::shutdownNode);
    cmd->mapCommand("cd", &FilesystemModule::changeDirectory);
    cmd->mapCommand("pwd", &FilesystemModule::printWorkingDirectory);
    cmd->mapCommand("ls", &FilesystemModule::listCurrentDirectory);
    cmd->mapCommand("scp", &FilesystemModule::remoteGetFile);
    cmd->mapCommand("createRep", &RepoModule::createRepository);
    cmd->mapCommand("selectRep", &RepoModule::selectRepository);
    cmd->mapCommand("saveRep", &RepoModule::saveRepository);
    cmd->mapCommand("loadRep", &RepoModule::loadRepository);
    cmd->mapCommand("printRep", &RepoModule::printHistory);
    cmd->mapCommand("downRep", &RepoModule::downloadRemoteRepository);
    cmd->mapCommand("resRep", &RepoModule::restoreRepository);
    cmd->mapCommand("updateAll", &RepoModule::updateAllFiles);
    cmd->mapCommand("deploy", &RepoModule::deployAllFiles);
    cmd->mapCommand("run", &CommandModule::runScript);
    cmd->mapCommand("sleep", &CommandModule::sleep);
    cmd->mapCommand("saveConf", &BasicModule::saveAllConfiguration);
    cmd->mapRawCommand("bg", &CommandModule::runInBackground);
    cmd->mapCommand("fireTrigV", &BasicModule::fireTriggerValue<std::string, std::string>);
    cmd->mapCommand("fireTrig", &BasicModule::fireTrigger<std::string>);

}

struct BscControlProgramParameters : CommandLineParameters {
    DefaultParameter<int> port = {
            {'P',
             "port",
             "PORT",
             "Port opened for incoming connections",
             9999}};//@todo control should not open any port, I should add client-only option for NetworkModule.
    DefaultParameter<std::string> networkName = {{'N', "network", "NETWORK", "Network id", "TheNetwork"}};
};

int main(int argc, char* argv[]) {

    auto parameters = CommandLineParser::defaultParse<BscControlProgramParameters>(argc, argv);
    Node thisNode;

//    auto host_name = boost::asio::ip::host_name();
    //@todo name from configuration
    thisNode.getNodeInfo().setNodeId(Poco::Environment::nodeName());

    setupModules(thisNode);
    thisNode.getModule<NetworkModule>()->addToNetwork(parameters.networkName());
    thisNode.getModule<NetworkModule>()->configuration().setPort(parameters.port());

    auto cmdN = thisNode.getModule<CommandModule>();

    setupCommands(cmdN.get());
    cmdN->setInteractive(false);

    thisNode.start();
    thisNode.waitToFinish();
    return 0;
}
