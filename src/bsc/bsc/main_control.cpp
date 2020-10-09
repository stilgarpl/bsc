#include "p2p/core/node/Node.h"
#include "p2p/modules/network/protocol/logic/actions/NetworkActions.h"
#include "p2p/modules/network/protocol/logic/actions/NodeActions.h"
#include "p2p/modules/network/protocol/logic/sources/NodeSource.h"
#include <iostream>
#include <p2p/modules/command/CommandModule.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

using namespace bsc;

using namespace std::chrono_literals;

void setupModules(bsc::Node& node) { node.addModule<CommandModule>(); }

struct BscControlProgramParameters : CommandLineParameters {
    Usage usage                        = {"COMMAND [COMMAND OPTIONS] [COMMAND ARGUMENTS]"};
    Usage us1                          = {{{"Is1"}, {"Is2"}}};
    DefaultParameter<bool> interactive = {
            {.shortKey = 's', .longKey = "shell", .doc = "Run in shell mode", .defaultValue = false}};
    DefaultParameter<int> port = {
            {'P',
             "port",
             "PORT",
             "Port opened for incoming connections",
             9191}};//@todo control should not open any port, I should add client-only option for NetworkModule.
    DefaultParameter<std::string> daemonAddress = {
            {'c', "connect", "ADDRESS", "Adress of daemon to control", "127.0.0.1:9999"}};
    DefaultParameter<std::string> networkName = {{'N', "network", "NETWORK", "Network id", "TheNetwork"}};
};

int main(int argc, char* argv[]) {
    //@todo hide initialization of spdlog in Node or sth.
    auto file_logger = spdlog::basic_logger_mt("basic_logger", "/tmp/bsc_control.log");
    spdlog::set_default_logger(file_logger);
    spdlog::flush_every(std::chrono::seconds(5));

    auto parameters = CommandLineParser::defaultParse<BscControlProgramParameters>(argc, argv);
    if (argc <= 1) {
        return 0;
    }
    bsc::Node thisNode;

    //@todo load node info from file? or sth?
    thisNode.getNodeInfo().setNodeId("control");

    setupModules(thisNode);
    thisNode.getModule<bsc::NetworkModule>()->addToNetwork(parameters.networkName());
    std::cout << "Port = " << std::to_string(parameters.port()) << std::endl;
    std::cout << "Address = " << parameters.daemonAddress() << std::endl;
    thisNode.getModule<bsc::NetworkModule>()->configuration().setPort(parameters.port());

    auto cmdN = thisNode.getModule<CommandModule>();
    cmdN->setInteractive(false);
    auto& commands = parameters.arguments();


    thisNode.start();
    thisNode.waitUntilStarted();
    try {
        auto& daemonNode = thisNode.getModule<bsc::NetworkModule>()->connectTo(parameters.daemonAddress());
        cmdN->sendCommandToRemoteNode(daemonNode, commands);
        daemonNode.disconnect();
    } catch (const bsc::RemoteNodeConnectionException& e) {
        //@todo formatting, localization and stuff...
        std::cerr << " Unable to connect to remote daemon node. Are you sure it's running?" << std::endl;
    }
    thisNode.stop();

    thisNode.waitToFinish();


    return 0;
}
