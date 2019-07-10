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


using namespace std::chrono_literals;

#include <variant>
#include <p2p/modules/basic/BasicModule.h>


void remoteServerTestModuleSetup(Node &node) {
    node.addModule<BasicModule>();
    node.addModule<NetworkModule>();
}

void waitFor(const std::function<bool(void)> &expression, std::chrono::milliseconds timeout) {

    auto beginTime = std::chrono::steady_clock::now();
    while (!expression() || std::chrono::steady_clock::now() - beginTime < timeout) {
        std::this_thread::sleep_for(10ms);
    }
}

#define INFO LOGGER
#define REQUIRE(x) if (!(x)) throw int(5);

int main(int argc, char *argv[]) {

    Node thisNode;

    thisNode.getNodeInfo().setNodeId("firstNodeR");

    remoteServerTestModuleSetup(thisNode);
    thisNode.getModule<NetworkModule>()->addToNetwork("TheNetwork");
    thisNode.getModule<NetworkModule>()->configuration().setPort(9191);

    Node otherNode;
    otherNode.getNodeInfo().setNodeId("secondR");
    remoteServerTestModuleSetup(otherNode);
    otherNode.getModule<NetworkModule>()->addToNetwork("TheNetwork");
    otherNode.getModule<NetworkModule>()->configuration().setPort(9192);

    thisNode.start();
    otherNode.start();

    thisNode.waitUntilStarted();
    otherNode.waitUntilStarted();

//SECTION("Remote test") {
    thisNode.setNodeContextActive();
    auto &remoteSecondNode = thisNode.getModule<NetworkModule>()->connectTo("127.0.0.1:9192");
    bool connectedToSecond = remoteSecondNode.isConnected();
    INFO("testing require")
    REQUIRE(connectedToSecond);
    waitFor([&] { return remoteSecondNode.getNodeId().has_value(); }, 5s);
    bool hasNodeId = remoteSecondNode.getNodeId().has_value();
    REQUIRE(hasNodeId);
    auto realNodeId = *remoteSecondNode.getNodeId();
    REQUIRE(realNodeId == "secondR");

    otherNode.setNodeContextActive();
    auto &firstRemoteNode = otherNode.getModule<NetworkModule>()->getRemoteNode("firstNodeR");
    auto serverSideNodeId = firstRemoteNode.getNodeId();
    REQUIRE(serverSideNodeId == "firstNodeR");
//}
//finish and cleanup
    std::this_thread::sleep_for(500ms);
    INFO("stopping")
    thisNode.stop();
    otherNode.stop();
    thisNode.waitToFinish();
    otherNode.waitToFinish();


}


