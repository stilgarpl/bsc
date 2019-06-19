//
// Created by stilgar on 19.06.19.
//

#include <catch2/catch.hpp>
#include <p2p/node/Node.h>
#include <p2p/modules/filesystem/FilesystemModule.h>

void setupModules(Node &node) {
    node.addModule<FilesystemModule>();
}


TEST_CASE("Transfer test", "[!throws]") {

    Node thisNode(9191);

    thisNode.getNodeInfo().setNodeId("first Node");

    setupModules(thisNode);
    thisNode.getModule<NodeNetworkModule>()->addToNetwork("TheNetwork");
    thisNode.getModule<NodeNetworkModule>()->configuration().setPort(9191);

    Node otherNode(9999);
    otherNode.getNodeInfo().setNodeId("second");
    setupModules(otherNode);
    otherNode.getModule<NodeNetworkModule>()->addToNetwork("TheNetwork");
    otherNode.getModule<NodeNetworkModule>()->configuration().setPort(9999);

    thisNode.start();
    otherNode.start();

    thisNode.waitUntilStarted();
    otherNode.waitUntilStarted();

    auto remoteSecondNode = thisNode.getModule<NodeNetworkModule>()->connectTo("127.0.0.1:9999");
    bool connectedToSecond = remoteSecondNode.isConnected();
    INFO("testing require")
    REQUIRE(connectedToSecond);

    //actual tests

    //@todo add many large files on one node and transfer them to the other node.



    //finish and cleanup
    INFO("stopping")
    thisNode.stop();
    otherNode.stop();
    thisNode.waitToFinish();
    otherNode.waitToFinish();

}