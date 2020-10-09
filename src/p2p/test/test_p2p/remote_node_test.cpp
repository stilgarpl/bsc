//
// Created by Krzysztof Tulidowicz on 25.06.19.
//

#include <catch2/catch.hpp>
#include <p2p/core/node/Node.h>
#include <p2p/modules/filesystem/FilesystemModule.h>
#include <p2p/modules/basic/BasicModule.h>

#include <chrono>
#include <testaid/testaid.h>
using namespace bsc;

void remoteServerTestModuleSetup(Node& node) {
    node.addModule<BasicModule>();
    node.addModule<NetworkModule>();
    node.addModule<FilesystemModule>();
}


TEST_CASE("Remote node test") {

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
//    //@todo without this, on heavy load, connection isn't listening yet before test begins. investigate why start() doesn't start network module to listening phase.
//    std::this_thread::sleep_for(500ms);
    thisNode.waitUntilStarted();
    otherNode.waitUntilStarted();

    SECTION("Remote test") {
        thisNode.setNodeContextActive();
        auto& remoteSecondNode = thisNode.getModule<NetworkModule>()->connectTo("127.0.0.1:9192");
        bool connectedToSecond = remoteSecondNode.isConnected();
        INFO("testing require")
        REQUIRE(connectedToSecond);
        testaid::waitFor([&] { return remoteSecondNode.getNodeId().has_value(); }, 5s);
        bool hasNodeId = remoteSecondNode.getNodeId().has_value();
        REQUIRE(hasNodeId);
        auto realNodeId = *remoteSecondNode.getNodeId();
        REQUIRE(realNodeId == "secondR");

        otherNode.setNodeContextActive();
        auto &firstRemoteNode = otherNode.getModule<NetworkModule>()->getRemoteNode("firstNodeR");
        auto serverSideNodeId = firstRemoteNode.getNodeId();
        REQUIRE(serverSideNodeId == "firstNodeR");
    }
    //finish and cleanup
//    std::this_thread::sleep_for(500ms);
    INFO("stopping")
    thisNode.stop();
    otherNode.stop();
    thisNode.waitToFinish();
    otherNode.waitToFinish();


}