//
// Created by stilgar on 24.06.2020.
//
#include <catch2/catch.hpp>
#include <p2p/core/node/Node.h>
#include <p2p/modules/basic/BasicModule.h>
#include <aide/Aide.h>

using namespace bsc;

void basicModuleSetup(Node& node) { node.addModule<BasicModule>(); }

TEST_CASE("BasicModule test") {
    aide::TestDir testDir;
    Node thisNode({.rootPath = testDir.getTestDirPath()});
    thisNode.getNodeInfo().setNodeId("firstNodeR");
    basicModuleSetup(thisNode);
    auto basicModule = thisNode.getModule<BasicModule>();
    REQUIRE(basicModule != nullptr);
    thisNode.start();
    REQUIRE(thisNode.isRunning() == true);
    SECTION("shutdown command") {
        basicModule->shutdownNode();
        REQUIRE(thisNode.isRunning() == false);
    }

    SECTION("saveAllConfiguration command") {
        REQUIRE(!fs::exists(testDir.getTestDirPath() / thisNode.getNodeInfo().getNodeId() / "config" / "basic.cfg"));
        basicModule->saveAllConfiguration();
        REQUIRE(fs::exists(testDir.getTestDirPath() / thisNode.getNodeInfo().getNodeId() / "config" / "basic.cfg"));
    }
    thisNode.stop();

    REQUIRE(thisNode.isRunning() == false);
}