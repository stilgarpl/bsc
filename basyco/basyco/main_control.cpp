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
#include <p2p/modules/network/protocol/packet/NodeInfoGroup.h>
#include <p2p/modules/auth/AuthModule.h>



using namespace std::chrono_literals;

#include <variant>
#include <p2p/modules/basic/BasicModule.h>
#include <p2p/modules/command/CommandModule.h>


void setupModules(Node &node) {
    node.addModule<CommandModule>();
}



int main(int argc, char *argv[]) {

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
    auto &daemonNode = thisNode.getModule<NetworkModule>()->connectTo("127.0.0.1:9999");
    //@todo find a better way to wait for connection
    cmdN->sendCommandToRemoteNode(daemonNode, commands);
    thisNode.stop();

    thisNode.waitToFinish();


    return 0;
}
