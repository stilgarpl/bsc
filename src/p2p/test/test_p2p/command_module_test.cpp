//
// Created by Krzysztof Tulidowicz on 24.06.2020.
//

#include <catch2/catch.hpp>
#include <p2p/core/node/Node.h>
#include <p2p/modules/basic/BasicModule.h>
#include <p2p/modules/command/CommandModule.h>
#include <testaid/testaid.h>

using namespace bsc;

struct TestingCommandParams : public CommandLineParameters {
    Parameter<int> a  = {{'a', "aaa", "NUM", "IntegerParameter"}};
    Argument<float> f = {"float"};
    Argument<int> i   = {"int"};
};

class CommandTestingModule : public NodeModuleDependent<CommandTestingModule, CommandModule> {
public:
    int value1 = 0;
    int value2 = 0;

    void setValue1(int v) { value1 = v; }

    void setValue2(int v) { value2 = v; }

    void setValue1And2(int v1, int v2) {
        value1 = v1;
        value2 = v2;
    }

    void setValue1And2FromParameter(const TestingCommandParams& params, int v1) {
        value1 = v1;
        value2 = params.i();
    }

    void rawCommand(CommandModule::ArgumentContainerTypeCRef arguments) {
        value1 = arguments.size();
        value2 = arguments.size();
    }

    void setupActions(SetupActionHelper& actionHelper) override {}
    bool assignActions(AssignActionHelper& actionHelper) override { return true; }
    bool setupSources(SetupSourceHelper& sourceHelper) override { return true; }

    explicit CommandTestingModule(INode& node) : NodeModuleDependent(node, "testing command") {}
};

void commandModuleSetup(Node& node) {
    node.addModule<BasicModule>();
    node.addModule<NetworkModule>();
    node.addModule<CommandTestingModule>();
    auto commandModule = node.addModule<CommandModule>();
    commandModule->mapCommand("shutdown", &BasicModule::shutdownNode);
    commandModule->mapCommand("saveConfig", &BasicModule::saveAllConfiguration);
    commandModule->group("group1").mapCommand("shutdown", &BasicModule::shutdownNode);

    commandModule->mapCommand("v1", &CommandTestingModule::setValue1);
    commandModule->mapCommand("v2", &CommandTestingModule::setValue2);
    commandModule->mapCommand("v", &CommandTestingModule::setValue1);
    commandModule->mapCommand("v", &CommandTestingModule::setValue1And2);
    commandModule->mapCommand("vr", &CommandTestingModule::setValue1);
    commandModule->mapCommand("vr", &CommandTestingModule::setValue1And2);
    commandModule->mapRawCommand("vr", &CommandTestingModule::rawCommand);
    commandModule->mapCommand("vp", &CommandTestingModule::setValue1And2FromParameter, {});
    commandModule->mapCommand("vp", &CommandTestingModule::setValue1);
}

TEST_CASE("CommandModule test") {
    testaid::TestDir testDir;
    Node thisNode({.rootPath = testDir.getTestDirPath()});
    thisNode.getNodeInfo().setNodeId("firstNodeR");
    commandModuleSetup(thisNode);
    auto commandModule = thisNode.getModule<CommandModule>();
    auto testingModule = thisNode.getModule<CommandTestingModule>();
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

    SECTION("overload set test") {
        SECTION("value setting") {
            REQUIRE(testingModule->value1 == 0);
            REQUIRE(testingModule->value2 == 0);
            auto result1 = commandModule->runCommand("v1", {"5"});
            REQUIRE(testingModule->value1 == 5);
            REQUIRE(testingModule->value2 == 0);
            REQUIRE(result1 == CommandModule::CommandExecutionStatus::success);
            auto result2 = commandModule->runCommand("v2", {"8"});
            REQUIRE(testingModule->value1 == 5);
            REQUIRE(testingModule->value2 == 8);
            REQUIRE(result2 == CommandModule::CommandExecutionStatus::success);
        }
        SECTION("simple overload") {
            REQUIRE(testingModule->value1 == 0);
            REQUIRE(testingModule->value2 == 0);
            SECTION("one argument") {
                auto result = commandModule->runCommand("v", {"5"});
                REQUIRE(testingModule->value1 == 5);
                REQUIRE(testingModule->value2 == 0);
                REQUIRE(result == CommandModule::CommandExecutionStatus::success);
            }
            SECTION("two arguments") {
                auto result = commandModule->runCommand("v", {"5", "9"});
                REQUIRE(testingModule->value1 == 5);
                REQUIRE(testingModule->value2 == 9);
                REQUIRE(result == CommandModule::CommandExecutionStatus::success);
            }
            SECTION("three arguments") {
                auto result = commandModule->runCommand("v", {"5", "9", "10"});
                REQUIRE(testingModule->value1 == 0);
                REQUIRE(testingModule->value2 == 0);
                REQUIRE(result == CommandModule::CommandExecutionStatus::incorrectArgumentsCount);
            }
        }

        SECTION("overload with raw command") {
            REQUIRE(testingModule->value1 == 0);
            REQUIRE(testingModule->value2 == 0);
            SECTION("one argument") {
                auto result = commandModule->runCommand("vr", {"5"});
                REQUIRE(testingModule->value1 == 5);
                REQUIRE(testingModule->value2 == 0);
                REQUIRE(result == CommandModule::CommandExecutionStatus::success);
            }
            SECTION("two arguments") {
                auto result = commandModule->runCommand("vr", {"5", "9"});
                REQUIRE(testingModule->value1 == 5);
                REQUIRE(testingModule->value2 == 9);
                REQUIRE(result == CommandModule::CommandExecutionStatus::success);
            }
            SECTION("three arguments") {
                auto result = commandModule->runCommand("vr", {"5", "9", "10"});
                REQUIRE(testingModule->value1 == 3);
                REQUIRE(testingModule->value2 == 3);
                REQUIRE(result == CommandModule::CommandExecutionStatus::success);
            }
        }

        SECTION("overloads with parameters") {
            REQUIRE(testingModule->value1 == 0);
            REQUIRE(testingModule->value2 == 0);
            SECTION("one argument") {
                auto result = commandModule->runCommand("vp", {"5"});
                REQUIRE(testingModule->value1 == 5);
                REQUIRE(testingModule->value2 == 0);
                REQUIRE(result == CommandModule::CommandExecutionStatus::success);
            }
            SECTION("two arguments") {
                auto result = commandModule->runCommand("vp", {"5", "9"});
                REQUIRE(testingModule->value1 == 0);
                REQUIRE(testingModule->value2 == 0);
                REQUIRE(result == CommandModule::CommandExecutionStatus::incorrectArgumentsCount);
            }
            SECTION("three arguments") {
                auto result = commandModule->runCommand("vp", {"5.2", "9", "10"});
                REQUIRE(result == CommandModule::CommandExecutionStatus::success);
                REQUIRE(testingModule->value1 == 10);
                REQUIRE(testingModule->value2 == 9);
            }
        }
    }

    thisNode.stop();

    REQUIRE(thisNode.isRunning() == false);
}