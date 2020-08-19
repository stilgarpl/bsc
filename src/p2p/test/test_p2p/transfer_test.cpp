//
// Created by Krzysztof Tulidowicz on 19.06.19.
//

#include <catch2/catch.hpp>
#include <p2p/core/node/Node.h>
#include <p2p/modules/filesystem/FilesystemModule.h>
#include <p2p/modules/basic/BasicModule.h>

using namespace bsc;

void setupModules(bsc::Node& node) {
    node.addModule<bsc::BasicModule>();
    node.addModule<bsc::NetworkModule>();
    node.addModule<FilesystemModule>();
}

void createFile(fs::path path, const std::string& content) {
    std::ofstream file(path);
    file << content;
}

std::string readFile(const fs::path& path) {
    std::ifstream t(path);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    return str;
}

const std::string longString = std::string(
        "fdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklf") +
                               "fdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklf" +
                               "fdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklf" +
                               "fdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklf" +
                               "fdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklf" +
                               "fdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklf" +
                               "fdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklf" +
                               "fdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklffdsfjsdlkfjsdjfsdkhfsdlkfjhdskfhjsdklf";

const std::string veryLongString =
        longString + longString + longString + longString + longString + longString + longString + longString +
        longString + longString + longString + longString + longString + longString + longString + longString +
        longString + longString + longString + longString + longString + longString + longString + longString +
        longString + longString + longString + longString + longString + longString + longString + longString +
        longString + longString + longString + longString + longString + longString + longString + longString +
        longString + longString + longString + longString + longString + longString + longString + longString +
        longString + longString;

const std::string extremelyLongString =
        veryLongString + veryLongString + veryLongString + veryLongString + veryLongString + veryLongString +
        veryLongString + veryLongString + veryLongString + veryLongString + veryLongString + veryLongString +
        veryLongString + veryLongString + veryLongString + veryLongString + veryLongString + veryLongString +
        veryLongString + veryLongString + veryLongString + veryLongString + veryLongString + veryLongString +
        veryLongString + veryLongString + veryLongString + veryLongString + veryLongString + veryLongString +
        veryLongString;


TEST_CASE("Transfer test") {

    const int TEST_FILES_NUM = 10;

    bsc::Node thisNode;

    thisNode.getNodeInfo().setNodeId("firstNode");

    setupModules(thisNode);
    thisNode.getModule<bsc::NetworkModule>()->addToNetwork("TheNetwork");
    thisNode.getModule<bsc::NetworkModule>()->configuration().setPort(9191);

    bsc::Node otherNode;
    otherNode.getNodeInfo().setNodeId("second");
    setupModules(otherNode);
    otherNode.getModule<bsc::NetworkModule>()->addToNetwork("TheNetwork");
    otherNode.getModule<bsc::NetworkModule>()->configuration().setPort(9192);

    thisNode.start();
    otherNode.start();

    thisNode.waitUntilStarted();
    otherNode.waitUntilStarted();
    //@todo if possible, wait for otherNode to start listening.
    std::this_thread::sleep_for(500ms);
    auto& remoteSecondNode = thisNode.getModule<bsc::NetworkModule>()->connectTo("127.0.0.1:9192");
    bool connectedToSecond = remoteSecondNode.isConnected();
    INFO("testing require")
    REQUIRE(connectedToSecond);
    std::this_thread::sleep_for(200ms);
    bool hasNodeId = remoteSecondNode.getNodeId().has_value();
    REQUIRE(hasNodeId);
    auto realNodeId = *remoteSecondNode.getNodeId();
    REQUIRE(realNodeId == "second");
    fs::path testPath = fs::temp_directory_path() / "one";
    fs::path destinationPath = fs::temp_directory_path() / "two";
    //@todo test is broken, it hangs on queue transfer. fix queue function so it doesn't broadcast immediately, but only on start of the transfer.
//    //actual tests
    SECTION("transfer test") {

        INFO("test path is " << testPath.string())
        fs::create_directories(testPath);
        fs::create_directories(destinationPath);
        for (int i = 0; i < TEST_FILES_NUM; ++i) {
            INFO("creating files for " + std::to_string(i))
            createFile(testPath / ("l" + std::to_string(i) + ".txt"), longString);
            createFile(testPath / ("v" + std::to_string(i) + ".txt"), veryLongString);
            createFile(testPath / ("e" + std::to_string(i) + ".txt"), extremelyLongString);
        }
        thisNode.setNodeContextActive();

        auto transferQueue = thisNode.getModule<FilesystemModule>()->transferQueue();

        for (int i = 0; i < TEST_FILES_NUM; ++i) {
            INFO("downloading files for " + std::to_string(i))
            transferQueue->queueTransfer(
                    std::make_shared<bsc::SimplePathRI>(testPath / ("l" + std::to_string(i) + ".txt")),
                    std::make_shared<bsc::SimplePathRI>(
                            destinationPath / ("l" + std::to_string(i) + ".txt")));
            transferQueue->queueTransfer(
                    std::make_shared<bsc::SimplePathRI>(testPath / ("v" + std::to_string(i) + ".txt")),
                    std::make_shared<bsc::SimplePathRI>(
                            destinationPath / ("v" + std::to_string(i) + ".txt")));
            transferQueue->queueTransfer(
                    std::make_shared<bsc::SimplePathRI>(testPath / ("e" + std::to_string(i) + ".txt")),
                    std::make_shared<bsc::SimplePathRI>(
                            destinationPath / ("e" + std::to_string(i) + ".txt")));
        }

        transferQueue->start();
        INFO("waiting for all transfers")
        transferQueue->waitToFinishAllTransfers();
        INFO("transfers complete, check downloaded files")

        for (int i = 0; i < TEST_FILES_NUM; ++i) {
            REQUIRE(readFile(destinationPath / ("l" + std::to_string(i) + ".txt")) == longString);
            REQUIRE(readFile(destinationPath / ("v" + std::to_string(i) + ".txt")) == veryLongString);
            REQUIRE(readFile(destinationPath / ("e" + std::to_string(i) + ".txt")) == extremelyLongString);
        }


    }


    //@todo add many large files on one node and transfer them to the other node.



    //finish and cleanup
    INFO("stopping")
    fs::remove_all(testPath);
    fs::remove_all(destinationPath);
    thisNode.stop();
    otherNode.stop();
    thisNode.waitToFinish();
    otherNode.waitToFinish();


}