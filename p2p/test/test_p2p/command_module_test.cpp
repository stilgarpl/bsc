//
// Created by Krzysztof Tulidowicz on 24.06.2020.
//

#include <aide/Aide.h>
#include <catch2/catch.hpp>
#include <p2p/core/node/Node.h>
#include <p2p/modules/basic/BasicModule.h>
#include <p2p/modules/command/CommandModule.h>

using namespace bsc;

void commandModuleSetup(Node& node) {
    node.addModule<BasicModule>();
    node.addModule<NetworkModule>();
    auto commandModule = node.addModule<CommandModule>();
    commandModule->mapCommand("shutdown", &BasicModule::shutdownNode);
    commandModule->mapCommand("saveConfig", &BasicModule::saveAllConfiguration);
    commandModule->group("group1").mapCommand("shutdown", &BasicModule::shutdownNode);
}

TEST_CASE("CommandModule test") {
    aide::TestDir testDir;
    Node thisNode({.rootPath = testDir.getTestDirPath()});
    thisNode.getNodeInfo().setNodeId("firstNodeR");
    commandModuleSetup(thisNode);
    auto commandModule = thisNode.getModule<CommandModule>();
    REQUIRE(commandModule != nullptr);
    thisNode.start();
    REQUIRE(thisNode.isRunning() == true);
    SECTION("shutdown command") {
        auto status = commandModule->runCommand("shutdown");
        REQUIRE(thisNode.isRunning() == false);
        REQUIRE(status == CommandModule::CommandExecutionStatus::success);
    }
    SECTION("saveAllConfiguration command") {
        REQUIRE(!fs::exists(testDir.getTestDirPath() / thisNode.getNodeInfo().getNodeId() / "config" / "basic.cfg"));
        REQUIRE(!fs::exists(testDir.getTestDirPath() / thisNode.getNodeInfo().getNodeId() / "config" / "network.cfg"));
        REQUIRE(!fs::exists(testDir.getTestDirPath() / thisNode.getNodeInfo().getNodeId() / "config" / "command.cfg"));
        auto status = commandModule->runCommand("saveConfig");
        REQUIRE(fs::exists(testDir.getTestDirPath() / thisNode.getNodeInfo().getNodeId() / "config" / "basic.cfg"));
        REQUIRE(fs::exists(testDir.getTestDirPath() / thisNode.getNodeInfo().getNodeId() / "config" / "network.cfg"));
        REQUIRE(fs::exists(testDir.getTestDirPath() / thisNode.getNodeInfo().getNodeId() / "config" / "command.cfg"));
        REQUIRE(status == CommandModule::CommandExecutionStatus::success);
    }

    SECTION("group shutdown command") {
        auto status = commandModule->runCommand("group1", {"shutdown"});
        REQUIRE(status == CommandModule::CommandExecutionStatus::success);
        REQUIRE(thisNode.isRunning() == false);
    }

    SECTION("group no command") {
        auto status = commandModule->runCommand("group1");
        REQUIRE(status == CommandModule::CommandExecutionStatus::noCommand);
    }

    SECTION("unknown command") {
        auto status = commandModule->runCommand("unknown command");
        REQUIRE(status == CommandModule::CommandExecutionStatus::badCommand);
    }

    SECTION("too many arguments command") {
        auto status = commandModule->runCommand("shutdown", {"argument"});
        REQUIRE(status == CommandModule::CommandExecutionStatus::tooManyArguments);
    }

    thisNode.stop();

    REQUIRE(thisNode.isRunning() == false);
}