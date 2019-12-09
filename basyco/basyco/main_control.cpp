#include <iostream>

//#define CEREAL_THREAD_SAFE 1
#include "p2p/node/Node.h"
#include "p2p/modules/network/protocol/logic/sources/NodeSource.h"
#include "p2p/modules/network/protocol/logic/actions/NodeActions.h"
#include "p2p/modules/network/protocol/logic/actions/NetworkActions.h"
#include <cereal/types/memory.hpp>
#include <p2p/modules/command/CommandModule.h>
#include <spdlog/sinks/basic_file_sink.h>


using namespace std::chrono_literals;


void setupModules(Node& node) {
    node.addModule<CommandModule>();
}

enum {
    a, b, c
};
enum QQ {
    x, y, z
};


int main(int argc, char* argv[]) {
    auto file_logger = spdlog::basic_logger_mt("basic_logger", "/tmp/basyco_control.log");
    spdlog::set_default_logger(file_logger);
    spdlog::flush_every(std::chrono::seconds(5));


    if (argc <= 1) {
        return 0;
    }
    Node thisNode;

    //@todo load node info from file? or sth?
    thisNode.getNodeInfo().setNodeId("control");

    setupModules(thisNode);
    thisNode.getModule<NetworkModule>()->addToNetwork("TheNetwork");
    thisNode.getModule<NetworkModule>()->configuration().setPort(9191);

    auto cmdN = thisNode.getModule<CommandModule>();
    cmdN->setInteractive(false);
    //@todo better way of running stuff from commandline
    std::vector<std::string> commands;
    for (int i = 1; i < argc; ++i) {
        commands.emplace_back(argv[i]);
    }


    thisNode.start();
    thisNode.waitUntilStarted();
//@todo get actual daemon address from configuration
    try {
        auto& daemonNode = thisNode.getModule<NetworkModule>()->connectTo("127.0.0.1:9999");
        cmdN->sendCommandToRemoteNode(daemonNode, commands);
        daemonNode.disconnect();
    } catch (const RemoteNodeConnectionException& e) {
        //@todo formatting, localization and stuff...
        std::cerr << " Unable to connect to remote daemon node. Are you sure it's running?" << std::endl;
    }
    thisNode.stop();

    thisNode.waitToFinish();


    return 0;
}
