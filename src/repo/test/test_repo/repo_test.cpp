//
// Created by Krzysztof Tulidowicz on 08.12.17.
//


#include <catch2/catch.hpp>
#include <p2p/modules/basic/BasicModule.h>
#include <p2p/modules/command/CommandModule.h>
#include <p2p/modules/filesystem/FilesystemModule.h>
#include <repo/node/RepoModule.h>
#include <testaid/testaid.h>

using namespace bsc;


void remoteServerTestModuleSetup(bsc::Node &node) {
    node.addModule<bsc::BasicModule>();
    node.addModule<FilesystemModule>();
    node.addModule<bsc::NetworkModule>();
    node.addModule<RepoModule>();
    node.addModule<CommandModule>();
}

void setupCommands(CommandModule *cmd) {
    cmd->mapCommand("t2", &CommandModule::testingMethodInt);
    cmd->group("tt").mapCommand("t2", &CommandModule::testingMethodInt);
    cmd->group("tt").group("xx").mapCommand("tx", &CommandModule::testingMethodInt);
    cmd->mapCommand("t3", &CommandModule::testingMethodIntFloat);
    cmd->mapCommand("connect", &bsc::NetworkModule::connectTo);
    cmd->mapCommand("connectTo", &bsc::NetworkModule::connectToNode);
    cmd->mapCommand<bsc::NetworkModule, RemoteNode&, const NodeIdType&>("getnode", &bsc::NetworkModule::getRemoteNode);
    cmd->mapCommand("disconnect", &bsc::NetworkModule::disconnect);
    cmd->mapCommand("print", &bsc::NetworkModule::printConnections);
//    cmd->mapCommand("update", &NetworkModule:prin:updateNodeConnectionInfo);
//    cmd->mapCommand("purgeD", &NetworkModule::purgeDuplicateConnections);
//    cmd->mapCommand("purgeI", &NetworkModule::purgeInactiveConnections);
    cmd->mapRawCommand("remote", &CommandModule::sendRemoteCommand);
    cmd->mapRawCommand("broadcast", &CommandModule::broadcastRemoteCommand);
    cmd->mapCommand("shutdown", &bsc::BasicModule::shutdownNode);
    cmd->mapCommand("cd", &FilesystemModule::changeDirectory);
    cmd->mapCommand("pwd", &FilesystemModule::printWorkingDirectory);
    cmd->mapCommand("ls", &FilesystemModule::listCurrentDirectory);
//    cmd->mapCommand("scp", &FilesystemModule::remoteGetFile);
    cmd->mapCommand("scp", &FilesystemModule::remoteGetFile);
//    cmd->mapCommand("lscp", &FilesystemModule::printCurrentTransfers);

    cmd->mapCommand("createRep", &RepoModule::createRepository);
    cmd->mapCommand("selectRep", &RepoModule::selectRepository);
    cmd->mapCommand("saveRep", &RepoModule::saveRepository);
    cmd->mapCommand("loadRep", &RepoModule::loadRepository);
    cmd->mapCommand("printRep", &RepoModule::printHistory);
    cmd->mapCommand("downRep", &RepoModule::downloadRemoteRepository);
    cmd->mapCommand("resRep", &RepoModule::restoreRepository);
    cmd->mapCommand("update", &RepoModule::updateFile);
    cmd->mapCommand("updateAll", &RepoModule::updateAllFiles);
    cmd->mapCommand("deploy", &RepoModule::deployAllFiles);
    cmd->mapCommand("run", &CommandModule::runScript);
    cmd->mapCommand("sleep", &CommandModule::sleep);
    cmd->mapRawCommand("bg", &CommandModule::runInBackground);
    cmd->mapCommand("fireTrigV", &bsc::BasicModule::fireTriggerValue<std::string, std::string>);
    cmd->mapCommand("fireTrig", &bsc::BasicModule::fireTrigger<std::string>);
//    cmd->mapCommand("requestResource", &RepoModule::requestStoragePath);
//    cmd->mapCommand("beginTransfer", &FilesystemModule::beginTransferTest);
//    cmd->submodule("help").mapCommand("cmdList",&CommandModule::listCommands);

}



TEST_CASE("Repo module test") {

    bsc::Node thisNode;

    thisNode.getNodeInfo().setNodeId("first Node");

    remoteServerTestModuleSetup(thisNode);
    thisNode.getModule<bsc::NetworkModule>()->addToNetwork("TheNetwork");
    thisNode.getModule<bsc::NetworkModule>()->configuration().setPort(9191);
    auto cmdN = thisNode.getModule<CommandModule>();

    setupCommands(cmdN.get());
    auto thisRepoMod = thisNode.getModule<RepoModule>();
    thisRepoMod->configuration().setAutoProcess(false);

    bsc::Node otherNode;
    otherNode.getNodeInfo().setNodeId("second");
    remoteServerTestModuleSetup(otherNode);
    auto otherRepoMod = otherNode.getModule<RepoModule>();
    otherRepoMod->configuration().setRepositoryDataPath("repo");
    otherRepoMod->configuration().setAutoProcess(false);

    otherNode.getModule<bsc::NetworkModule>()->addToNetwork("TheNetwork");
    otherNode.getModule<bsc::NetworkModule>()->configuration().setPort(9999);
    cmdN = otherNode.getModule<CommandModule>();
    setupCommands(cmdN.get());

    thisNode.start();
    otherNode.start();
    auto& secondNode = thisNode.getModule<bsc::NetworkModule>()->connectTo("127.0.0.1:9999");
    bool connectedToSecond = secondNode.isConnected();

    REQUIRE(connectedToSecond);

//    SECTION ("create repository on second ") {
    otherNode.setNodeContextActive();
    REQUIRE(otherRepoMod != nullptr);
    otherRepoMod->createRepository("test");
    REQUIRE(otherRepoMod->findRepository("test") != nullptr);
    otherRepoMod->selectRepository("test");
    REQUIRE(otherRepoMod->getSelectedRepository() != nullptr);

    //create dirs and files
    testaid::TestDir testDir;
    using namespace std::string_literals;
    fs::path testPath = testDir.getTestDirPath("dirtest"s);
    INFO("test path is " << testPath.string())
    testaid::createFile(testPath / "1.txt", "111");
    testaid::createFile(testPath / "2.txt", "222");
    testaid::createFile(testPath / "3.txt", "333");
    testaid::createFile(testPath / "4.txt", "444");
    fs::path subPath = testPath / "subdirectory" / "sub1";
    fs::create_directories(subPath);
    testaid::createFile(subPath / "sub.txt", "sub");

    otherNode.setNodeContextActive();
    otherRepoMod->updateFile(testPath);
    otherRepoMod->saveRepository("test");

    auto num = fs::remove_all(testPath);

    REQUIRE(num == 8);
    //@todo set the path from the actual configuration setDirect in this test

    auto repoXMLPath = otherNode.getConfigurationManager().getDataPath() / "repo" / "test.xml";
    std::cout << repoXMLPath << std::endl;
    INFO("root path is " << repoXMLPath.string())
    REQUIRE(fs::exists(repoXMLPath));
    otherNode.setNodeContextActive();
    auto otherSum = otherRepoMod->getSelectedRepository()->getJournal()->getChecksum();


//        SECTION("download remote repository") {
    REQUIRE(!fs::exists(testPath));
    REQUIRE(!fs::exists(testPath / "1.txt"));
    REQUIRE(!fs::exists(testPath / "2.txt"));
    REQUIRE(!fs::exists(testPath / "3.txt"));
    REQUIRE(!fs::exists(testPath / "4.txt"));
    REQUIRE(!fs::exists(subPath / "sub.txt"));
    thisNode.setNodeContextActive();

    bool noTestRepo = thisRepoMod->findRepository("test") != nullptr;
    REQUIRE_FALSE(noTestRepo);
    REQUIRE(secondNode.isConnected());
    //wait for the logic to setup remote node ids and stuff
    std::this_thread::sleep_for(500ms);
    thisRepoMod->downloadRemoteRepository("second", "test");
    bool okTestRepo = thisRepoMod->findRepository("test") != nullptr;
    REQUIRE(okTestRepo);

    thisRepoMod->selectRepository("test");
    REQUIRE(thisRepoMod->getSelectedRepository() != nullptr);
    REQUIRE(thisRepoMod->getSelectedRepository()->getRepositoryId() == "test");

    auto thisSum = thisRepoMod->getSelectedRepository()->getJournal()->getChecksum();
    REQUIRE(thisSum == otherSum);
    std::this_thread::sleep_for(500ms);
    thisRepoMod->deployAllFiles();
    std::this_thread::sleep_for(500ms);
    REQUIRE(fs::exists(testPath));
    REQUIRE(fs::exists(testPath / "1.txt"));
    REQUIRE(fs::exists(testPath / "2.txt"));
    REQUIRE(fs::exists(testPath / "3.txt"));
    REQUIRE(fs::exists(testPath / "4.txt"));
    REQUIRE(fs::exists(subPath / "sub.txt"));

//            SECTION("add, change, delete") {
    INFO("changing files")
    fs::remove(testPath / "4.txt");

    //@todo here I have a bug that looks like a race. probably transfer queue enters finished state too soon. investigate.
    //  after investigation : downloadStorage finished before transfer queue has actually finished. I think transfer queue goes to FINISHED state too soon.
    testaid::changeFile(testPath / "3.txt", "QWQQQQQQQQQ");
    testaid::createFile(testPath / "5.txt", "555");
    otherNode.setNodeContextActive();
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
    thisNode.setNodeContextActive();
    thisRepoMod->downloadRemoteRepository("second", "test");
    REQUIRE(thisRepoMod->findRepository("test") != nullptr);
    thisRepoMod->selectRepository("test");
    logger.debug("deplying files");
    thisRepoMod->deployAllFiles();

    REQUIRE(fs::exists(testPath));
    REQUIRE(fs::exists(testPath / "1.txt"));
    REQUIRE(fs::exists(testPath / "2.txt"));
    REQUIRE(fs::exists(testPath / "3.txt"));
    REQUIRE(!fs::exists(testPath / "4.txt"));
    REQUIRE(fs::exists(testPath / "5.txt"));
    REQUIRE(testaid::readFile(testPath / "1.txt") == "111");
    REQUIRE(testaid::readFile(testPath / "2.txt") == "222");
    REQUIRE(testaid::readFile(testPath / "3.txt") == "QWQQQQQQQQQ");
    REQUIRE(fs::exists(subPath / "sub.txt"));


//            }


//        }

    //cleanup:
    fs::remove_all(testPath);
    fs::remove_all({otherNode.getConfigurationManager().getDataPath()});

//    }

    INFO("closing");
    thisNode.setNodeContextActive();
    thisNode.stop();
    thisNode.waitToFinish();
    otherNode.setNodeContextActive();
    otherNode.stop();
    otherNode.waitToFinish();


}


TEST_CASE("Repo Integration Test") {
    //@todo implement
}