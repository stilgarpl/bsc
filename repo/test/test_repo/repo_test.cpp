//
// Created by stilgar on 08.12.17.
//

#include <catch2/catch.hpp>
#include <p2p/modules/command/CommandModule.h>
#include <repo/node/RepoModule.h>
#include <p2p/modules/filesystem/FilesystemModule.h>
#include <p2p/modules/basic/BasicModule.h>
#include <p2p/modules/auth/AuthModule.h>

void remoteServerTestModuleSetup(Node &node) {
    node.addModule<BasicModule>();
    node.addModule<FilesystemModule>();
    node.addModule<NodeNetworkModule>();
    node.addModule<RepoModule>();
    node.addModule<CommandModule>();
}

void setupCommands(CommandModule *cmd) {
    cmd->mapCommand("t2", &CommandModule::testingMethodInt);
    cmd->submodule("tt").mapCommand("t2", &CommandModule::testingMethodInt);
    cmd->submodule("tt").submodule("xx").mapCommand("tx", &CommandModule::testingMethodInt);
    cmd->mapCommand("t3", &CommandModule::testingMethodIntFloat);
    cmd->mapCommand("connect", &NodeNetworkModule::connectTo);
    cmd->mapCommand("connectTo", &NodeNetworkModule::connectToNode);
    cmd->mapCommand<NodeNetworkModule, RemoteNode &, const NodeIdType &>("getnode", &NodeNetworkModule::getRemoteNode);
    cmd->mapCommand("disconnect", &NodeNetworkModule::disconnect);
    cmd->mapCommand("print", &NodeNetworkModule::printConnections);
//    cmd->mapCommand("update", &NodeNetworkModule:prin:updateNodeConnectionInfo);
//    cmd->mapCommand("purgeD", &NodeNetworkModule::purgeDuplicateConnections);
//    cmd->mapCommand("purgeI", &NodeNetworkModule::purgeInactiveConnections);
    cmd->mapRawCommand("remote", &CommandModule::sendRemoteCommand);
    cmd->mapRawCommand("broadcast", &CommandModule::broadcastRemoteCommand);
    cmd->mapCommand("shutdown", &BasicModule::shutdownNode);
    cmd->mapCommand("cd", &FilesystemModule::changeDirectory);
    cmd->mapCommand("pwd", &FilesystemModule::printWorkingDirectory);
    cmd->mapCommand("ls", &FilesystemModule::listCurrentDirectory);
//    cmd->mapCommand("scp", &FilesystemModule::remoteGetFile);
    cmd->mapCommand("scp", &FilesystemModule::remoteGetFile);
//    cmd->mapCommand("lscp", &FilesystemModule::printCurrentTransfers);

    cmd->mapCommand("createRep", &RepoModule::createRepository);
    cmd->mapCommand("selectRep", &RepoModule::selectRepository);
    cmd->mapCommand<RepoModule, void, const fs::path &>("persist", &RepoModule::persistFile);
    cmd->mapCommand("saveRep", &RepoModule::saveRepository);
    cmd->mapCommand("loadRep", &RepoModule::loadRepository);
    cmd->mapCommand("printRep", &RepoModule::printHistory);
    cmd->mapCommand("downRep", &RepoModule::downloadRemoteRepository);
    cmd->mapCommand("resRep", &RepoModule::restoreRepository);
    cmd->mapCommand("update", &RepoModule::updateFile);
    cmd->mapCommand("ignore", &RepoModule::ignoreFile);
    cmd->mapCommand("updateAll", &RepoModule::updateAllFiles);
    cmd->mapCommand("deploy", &RepoModule::deployAllFiles);
    cmd->mapCommand("run", &CommandModule::runScript);
    cmd->mapCommand("sleep", &CommandModule::sleep);
    cmd->mapRawCommand("bg", &CommandModule::runInBackground);
    cmd->mapCommand("fireTrigV", &BasicModule::fireTriggerValue<std::string, std::string>);
    cmd->mapCommand("fireTrig", &BasicModule::fireTrigger<std::string>);
//    cmd->mapCommand("requestResource", &RepoModule::requestStoragePath);
//    cmd->mapCommand("beginTransfer", &FilesystemModule::beginTransferTest);
//    cmd->submodule("help").mapCommand("cmdList",&CommandModule::listCommands);

}

void createFile(fs::path path, std::string content) {
    std::ofstream file(path);
    file << content;
}

void changeFile(fs::path path, std::string content) {
    std::ofstream file(path);
    file << content;
}

std::string readFile(fs::path path) {
    std::ifstream t(path);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    return str;
}

TEST_CASE("Repo module test", "[!throws]") {

    Node thisNode(9191);

    thisNode.getNodeInfo().setNodeId("first Node");

    remoteServerTestModuleSetup(thisNode);
    thisNode.getModule<NodeNetworkModule>()->addToNetwork("TheNetwork");
    thisNode.getModule<NodeNetworkModule>()->configuration().setPort(9191);
    auto cmdN = thisNode.getModule<CommandModule>();

    setupCommands(cmdN.get());

    thisNode.start();

    Node otherNode(9999);
    otherNode.getNodeInfo().setNodeId("second");


    remoteServerTestModuleSetup(otherNode);
    otherNode.getModule<NodeNetworkModule>()->addToNetwork("TheNetwork");
    otherNode.getModule<NodeNetworkModule>()->configuration().setPort(9999);
    cmdN = otherNode.getModule<CommandModule>();
    setupCommands(cmdN.get());

    otherNode.start();
    thisNode.waitUntilStarted();
    otherNode.waitUntilStarted();
    auto &secondNode = thisNode.getModule<NodeNetworkModule>()->connectTo("127.0.0.1:9999");
    bool connectedToSecond = secondNode.isConnected();

    REQUIRE(connectedToSecond);

    SECTION ("create repository on second ") {
        auto otherRepoMod = otherNode.getModule<RepoModule>();
        otherRepoMod->configuration().setRepositoryDataPath("repo");
        REQUIRE(otherRepoMod != nullptr);
        otherRepoMod->createRepository("test");
        REQUIRE(otherRepoMod->findRepository("test") != nullptr);
        otherRepoMod->selectRepository("test");
        REQUIRE(otherRepoMod->getSelectedRepository() != nullptr);

        //create dirs and files
        fs::path testPath = fs::temp_directory_path() / "dirtest";
        INFO("test path is " << testPath.string())
        fs::create_directories(testPath);
        createFile(testPath / "1.txt", "111");
        createFile(testPath / "2.txt", "222");
        createFile(testPath / "3.txt", "333");
        createFile(testPath / "4.txt", "444");
        fs::path subPath = testPath / "subdirectory" / "sub1";
        fs::create_directories(subPath);
        createFile(subPath / "sub.txt", "sub");


        otherRepoMod->updateFile(testPath);
        otherRepoMod->saveRepository("test");

        auto num = fs::remove_all(testPath);

        REQUIRE(num == 8);
        //@todo set the path from the actual configuration set in this test

        auto repoXMLPath = otherNode.getConfigurationManager().getDataPath() / "repo" / "test.xml";
        std::cout << repoXMLPath << std::endl;
        INFO("root path is " << repoXMLPath.string())
        REQUIRE(fs::exists(repoXMLPath));

        auto otherSum = otherRepoMod->getSelectedRepository()->getJournal()->getChecksum();


        SECTION("download remote repository") {
            REQUIRE(!fs::exists(testPath));
            REQUIRE(!fs::exists(testPath / "1.txt"));
            REQUIRE(!fs::exists(testPath / "2.txt"));
            REQUIRE(!fs::exists(testPath / "3.txt"));
            REQUIRE(!fs::exists(testPath / "4.txt"));
            REQUIRE(!fs::exists(subPath / "sub.txt"));

            auto thisRepoMod = thisNode.getModule<RepoModule>();

            REQUIRE_FALSE(thisRepoMod->findRepository("test") != nullptr);

            thisRepoMod->downloadRemoteRepository("second", "test");
            REQUIRE(thisRepoMod->findRepository("test") != nullptr);

            thisRepoMod->selectRepository("test");
            REQUIRE(thisRepoMod->getSelectedRepository() != nullptr);
            REQUIRE(thisRepoMod->getSelectedRepository()->getRepositoryId() == "test");

            auto thisSum = thisRepoMod->getSelectedRepository()->getJournal()->getChecksum();
            REQUIRE(thisSum == otherSum);

            thisRepoMod->deployAllFiles();

            REQUIRE(fs::exists(testPath));
            REQUIRE(fs::exists(testPath / "1.txt"));
            REQUIRE(fs::exists(testPath / "2.txt"));
            REQUIRE(fs::exists(testPath / "3.txt"));
            REQUIRE(fs::exists(testPath / "4.txt"));
            REQUIRE(fs::exists(subPath / "sub.txt"));

            SECTION("add, change, delete") {
                fs::remove(testPath / "4.txt");
                changeFile(testPath / "3.txt", "QWQQQQQQQQQ");
                createFile(testPath / "5.txt", "555");
                otherRepoMod->updateAllFiles();
                otherRepoMod->saveRepository("test");

                auto num2 = fs::remove_all(testPath);
                REQUIRE(num2 == 8);
                REQUIRE(!fs::exists(testPath));
                REQUIRE(!fs::exists(testPath / "1.txt"));
                REQUIRE(!fs::exists(testPath / "2.txt"));
                REQUIRE(!fs::exists(testPath / "3.txt"));
                REQUIRE(!fs::exists(testPath / "4.txt"));
                REQUIRE(!fs::exists(testPath / "5.txt"));
                thisRepoMod->downloadRemoteRepository("second", "test");
                REQUIRE(thisRepoMod->findRepository("test") != nullptr);
                thisRepoMod->selectRepository("test");
                thisRepoMod->deployAllFiles();

                REQUIRE(fs::exists(testPath));
                REQUIRE(fs::exists(testPath / "1.txt"));
                REQUIRE(fs::exists(testPath / "2.txt"));
                REQUIRE(fs::exists(testPath / "3.txt"));
                REQUIRE(!fs::exists(testPath / "4.txt"));
                REQUIRE(fs::exists(testPath / "5.txt"));
                REQUIRE(readFile(testPath / "3.txt") == "QWQQQQQQQQQ");
                REQUIRE(fs::exists(subPath / "sub.txt"));


            }


        }

        //@todo implement: (passing commands as user would)
//        SECTION("integration command test") {
//            REQUIRE(!fs::exists(testPath));
//            REQUIRE(!fs::exists(testPath / "1.txt"));
//            REQUIRE(!fs::exists(testPath / "2.txt"));
//            REQUIRE(!fs::exists(testPath / "3.txt"));
//            REQUIRE(!fs::exists(testPath / "4.txt"));
//
//
//            REQUIRE(fs::exists(testPath));
//            REQUIRE(fs::exists(testPath / "1.txt"));
//            REQUIRE(fs::exists(testPath / "2.txt"));
//            REQUIRE(fs::exists(testPath / "3.txt"));
//            REQUIRE(fs::exists(testPath / "4.txt"));
//
//
//        }


        //cleanup:
        fs::remove_all(testPath);

    }

    INFO("closing");

    thisNode.stop();
    thisNode.waitToFinish();
    otherNode.stop();
    otherNode.waitToFinish();


}


TEST_CASE("Repo Integration Test", "[!throws]") {
    //@todo implement
}