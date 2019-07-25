//
// Created by stilgar on 25.06.19.
//

#include <catch2/catch.hpp>
#include <p2p/node/Node.h>
#include <p2p/modules/filesystem/FilesystemModule.h>
#include <p2p/modules/basic/BasicModule.h>

#include <chrono>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void remoteServerTestModuleSetup(Node &node) {
    node.addModule<BasicModule>();
    node.addModule<NetworkModule>();
    node.addModule<FilesystemModule>();
}

void waitFor(const std::function<bool(void)> &expression, std::chrono::milliseconds timeout) {

    auto beginTime = std::chrono::steady_clock::now();
    while (!expression() || std::chrono::steady_clock::now() - beginTime < timeout) {
        std::this_thread::sleep_for(10ms);
    }
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

    thisNode.waitUntilStarted();
    otherNode.waitUntilStarted();

    SECTION("Remote test") {
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
    }
    //finish and cleanup
    std::this_thread::sleep_for(500ms);
    INFO("stopping")
    thisNode.stop();
    otherNode.stop();
    thisNode.waitToFinish();
    otherNode.waitToFinish();


}