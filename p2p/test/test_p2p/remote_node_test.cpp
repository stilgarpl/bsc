//
// Created by stilgar on 25.06.19.
//

#include <catch2/catch.hpp>
#include <p2p/node/Node.h>
#include <p2p/modules/filesystem/FilesystemModule.h>
#include <p2p/modules/basic/BasicModule.h>

void remoteServerTestModuleSetup(Node &node) {
    node.addModule<NodeNetworkModule>();
}


TEST_CASE("Remote node test", "[!throws]") {


    Node thisNode(9191);

    thisNode.getNodeInfo().setNodeId("firstNode");

    remoteServerTestModuleSetup(thisNode);
    thisNode.getModule<NodeNetworkModule>()->addToNetwork("TheNetwork");
    thisNode.getModule<NodeNetworkModule>()->configuration().setPort(9191);

    Node otherNode(9192);
    otherNode.getNodeInfo().setNodeId("second");
    remoteServerTestModuleSetup(otherNode);
    otherNode.getModule<NodeNetworkModule>()->addToNetwork("TheNetwork");
    otherNode.getModule<NodeNetworkModule>()->configuration().setPort(9192);

    thisNode.start();
    otherNode.start();

    thisNode.waitUntilStarted();
    otherNode.waitUntilStarted();

    SECTION("Remote test") {
        auto &remoteSecondNode = thisNode.getModule<NodeNetworkModule>()->connectTo("127.0.0.1:9192");
        bool connectedToSecond = remoteSecondNode.isConnected();
        INFO("testing require")
        REQUIRE(connectedToSecond);
        std::this_thread::sleep_for(200ms);
        bool hasNodeId = remoteSecondNode.getNodeId().has_value();
        REQUIRE(hasNodeId);
        auto realNodeId = *remoteSecondNode.getNodeId();
        REQUIRE(realNodeId == "second");


        auto &firstRemoteNode = otherNode.getModule<NodeNetworkModule>()->getRemoteNode("firstNode");
        auto serverSideNodeId = firstRemoteNode.getNodeId();
        REQUIRE(serverSideNodeId == "firstNode");
    }
    //finish and cleanup
    INFO("stopping")
    thisNode.stop();
    otherNode.stop();
    thisNode.waitToFinish();
    otherNode.waitToFinish();


}