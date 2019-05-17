//
// Created by stilgar on 08.12.17.
//

#include <catch2/catch.hpp>
#include <p2p/modules/command/CommandModule.h>
#include <repo/node/RepoModule.h>
#include <p2p/modules/filesystem/FilesystemModule.h>
#include <p2p/modules/basic/BasicModule.h>
#include <p2p/modules/auth/AuthModule.h>

void setupModules(Node &node) {
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


TEST_CASE("Repo module test", "[!throws]") {

    Node thisNode(9191);

    thisNode.getNodeInfo().setNodeId("first Node");

    setupModules(thisNode);
    thisNode.getModule<NodeNetworkModule>()->addToNetwork("TheNetwork");
    thisNode.getModule<NodeNetworkModule>()->configuration().setPort(9191);
    auto cmdN = thisNode.getModule<CommandModule>();

    setupCommands(cmdN.get());
//    cmdN->setInteractive(true);

    thisNode.start();

    Node otherNode(9999);
    otherNode.getNodeInfo().setNodeId("second");


    setupModules(otherNode);
    otherNode.getModule<NodeNetworkModule>()->addToNetwork("TheNetwork");
    otherNode.getModule<NodeNetworkModule>()->configuration().setPort(9999);
    cmdN = otherNode.getModule<CommandModule>();
    setupCommands(cmdN.get());

    otherNode.start();

    std::this_thread::sleep_for(2s);
    thisNode.getModule<NodeNetworkModule>()->connectTo("127.0.0.1:9999");
    std::this_thread::sleep_for(2s);
    bool connectedToSecond = thisNode.getModule<NodeNetworkModule>()->getRemoteNode("second").isConnected();

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
        fs::create_directories(testPath);
        createFile(testPath / "1.txt", "111");
        createFile(testPath / "2.txt", "222");
        createFile(testPath / "3.txt", "333");
        createFile(testPath / "4.txt", "444");


        otherRepoMod->persistFile(testPath);
        otherRepoMod->saveRepository("test");

        auto num = fs::remove_all(testPath);

        REQUIRE(num == 5);
        //@todo set the path from the actual configuration set in this test
        auto repoXMLPath = fs::temp_directory_path() / "basyco/second/data/repo/test.xml";
        REQUIRE(fs::exists(repoXMLPath));

        auto otherSum = otherRepoMod->getSelectedRepository()->getJournal()->getChecksum();


        SECTION("download remote repository") {
            REQUIRE(!fs::exists(testPath));
            REQUIRE(!fs::exists(testPath / "1.txt"));
            REQUIRE(!fs::exists(testPath / "2.txt"));
            REQUIRE(!fs::exists(testPath / "3.txt"));
            REQUIRE(!fs::exists(testPath / "4.txt"));

            auto thisRepoMod = thisNode.getModule<RepoModule>();

            REQUIRE_FALSE(thisRepoMod->findRepository("test") != nullptr);

            thisRepoMod->downloadRemoteRepository("second", "test");
            REQUIRE(thisRepoMod->findRepository("test") != nullptr);

            thisRepoMod->selectRepository("test");
            REQUIRE(thisRepoMod->getSelectedRepository() != nullptr);

            auto thisSum = thisRepoMod->getSelectedRepository()->getJournal()->getChecksum();
            REQUIRE(thisSum == otherSum);

            thisRepoMod->deployAllFiles();

            REQUIRE(fs::exists(testPath));
            REQUIRE(fs::exists(testPath / "1.txt"));
            REQUIRE(fs::exists(testPath / "2.txt"));
            REQUIRE(fs::exists(testPath / "3.txt"));
            REQUIRE(fs::exists(testPath / "4.txt"));


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


    }

    WARN("closing");

    thisNode.stop();
    thisNode.waitToFinish();
    otherNode.stop();
    otherNode.waitToFinish();
    //@todo stop() should join all threads, including poco ones. stopping the app before it has finished crashes the app
    std::this_thread::sleep_for(5s);


}


TEST_CASE("Repo Integration Test", "[!throws]") {
    //@todo implement
}