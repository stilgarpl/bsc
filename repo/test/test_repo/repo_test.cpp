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
    node.addModule<NetworkModule>();
    node.addModule<RepoModule>();
    node.addModule<CommandModule>();
}

void setupCommands(CommandModule *cmd) {
    cmd->mapCommand("t2", &CommandModule::testingMethodInt);
    cmd->submodule("tt").mapCommand("t2", &CommandModule::testingMethodInt);
    cmd->submodule("tt").submodule("xx").mapCommand("tx", &CommandModule::testingMethodInt);
    cmd->mapCommand("t3", &CommandModule::testingMethodIntFloat);
    cmd->mapCommand("connect", &NetworkModule::connectTo);
    cmd->mapCommand("connectTo", &NetworkModule::connectToNode);
    cmd->mapCommand<NetworkModule, RemoteNode &, const NodeIdType &>("getnode", &NetworkModule::getRemoteNode);
    cmd->mapCommand("disconnect", &NetworkModule::disconnect);
    cmd->mapCommand("print", &NetworkModule::printConnections);
//    cmd->mapCommand("update", &NetworkModule:prin:updateNodeConnectionInfo);
//    cmd->mapCommand("purgeD", &NetworkModule::purgeDuplicateConnections);
//    cmd->mapCommand("purgeI", &NetworkModule::purgeInactiveConnections);
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
    if (fs::exists(path)) {
        std::ifstream t(path);
        std::string str((std::istreambuf_iterator<char>(t)),
                        std::istreambuf_iterator<char>());
        return str;
    } else {
        throw std::string("file not found");
    }
}

class Cleanup {
    fs::path pathToCleanup;
public:
    Cleanup(const fs::path &pathToCleanup) : pathToCleanup(pathToCleanup) {

    }

    virtual ~Cleanup() {
        fs::remove_all(pathToCleanup);
    }
};

TEST_CASE("Repo module test") {

    Node thisNode;

    thisNode.getNodeInfo().setNodeId("first Node");

    remoteServerTestModuleSetup(thisNode);
    thisNode.getModule<NetworkModule>()->addToNetwork("TheNetwork");
    thisNode.getModule<NetworkModule>()->configuration().setPort(9191);
    auto cmdN = thisNode.getModule<CommandModule>();

    setupCommands(cmdN.get());
    auto thisRepoMod = thisNode.getModule<RepoModule>();
    thisRepoMod->configuration().setAutoProcess(false);

    Node otherNode;
    otherNode.getNodeInfo().setNodeId("second");
    remoteServerTestModuleSetup(otherNode);
    auto otherRepoMod = otherNode.getModule<RepoModule>();
    otherRepoMod->configuration().setRepositoryDataPath("repo");
    otherRepoMod->configuration().setAutoProcess(false);

    otherNode.getModule<NetworkModule>()->addToNetwork("TheNetwork");
    otherNode.getModule<NetworkModule>()->configuration().setPort(9999);
    cmdN = otherNode.getModule<CommandModule>();
    setupCommands(cmdN.get());

    thisNode.start();
    otherNode.start();
    thisNode.waitUntilStarted();
    otherNode.waitUntilStarted();
    auto &secondNode = thisNode.getModule<NetworkModule>()->connectTo("127.0.0.1:9999");
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
    fs::path testPath = fs::temp_directory_path() / "dirtest";
    Cleanup cleanup(testPath);
    INFO("test path is " << testPath.string())
    //just in case, cleanup
    fs::remove_all(testPath);
    //create test directories
    fs::create_directories(testPath);
    createFile(testPath / "1.txt", "111");
    createFile(testPath / "2.txt", "222");
    createFile(testPath / "3.txt", "333");
    createFile(testPath / "4.txt", "444");
    fs::path subPath = testPath / "subdirectory" / "sub1";
    fs::create_directories(subPath);
    createFile(subPath / "sub.txt", "sub");

    otherNode.setNodeContextActive();
    otherRepoMod->updateFile(testPath);
    otherRepoMod->saveRepository("test");

    auto num = fs::remove_all(testPath);

    REQUIRE(num == 8);
    //@todo set the path from the actual configuration set in this test

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
    changeFile(testPath / "3.txt", "QWQQQQQQQQQ");
    createFile(testPath / "5.txt", "555");
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
    LOGGER("deplying files")
    thisRepoMod->deployAllFiles();

    REQUIRE(fs::exists(testPath));
    REQUIRE(fs::exists(testPath / "1.txt"));
    REQUIRE(fs::exists(testPath / "2.txt"));
    REQUIRE(fs::exists(testPath / "3.txt"));
    REQUIRE(!fs::exists(testPath / "4.txt"));
    REQUIRE(fs::exists(testPath / "5.txt"));
    REQUIRE(readFile(testPath / "1.txt") == "111");
    REQUIRE(readFile(testPath / "2.txt") == "222");
    REQUIRE(readFile(testPath / "3.txt") == "QWQQQQQQQQQ");
    REQUIRE(fs::exists(subPath / "sub.txt"));


//            }


//        }

    //cleanup:
    fs::remove_all(testPath);
    //@todo actual path or cleanup from storage
    fs::remove_all({"/tmp/storage"});

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