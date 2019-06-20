//
// Created by stilgar on 19.06.19.
//

#include <catch2/catch.hpp>
#include <p2p/node/Node.h>
#include <p2p/modules/filesystem/FilesystemModule.h>
#include <p2p/modules/basic/BasicModule.h>

void setupModules(Node &node) {
    node.addModule<BasicModule>();
    node.addModule<NodeNetworkModule>();
    node.addModule<FilesystemModule>();
}

void createFile(fs::path path, std::string content) {
    std::ofstream file(path);
    file << content;
}

std::string readFile(fs::path path) {
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


TEST_CASE("Transfer test", "[!throws]") {

    Node thisNode(9191);

    thisNode.getNodeInfo().setNodeId("firstNode");

    setupModules(thisNode);
    thisNode.getModule<NodeNetworkModule>()->addToNetwork("TheNetwork");
    thisNode.getModule<NodeNetworkModule>()->configuration().setPort(9191);

    Node otherNode(9192);
    otherNode.getNodeInfo().setNodeId("second");
    setupModules(otherNode);
    otherNode.getModule<NodeNetworkModule>()->addToNetwork("TheNetwork");
    otherNode.getModule<NodeNetworkModule>()->configuration().setPort(9192);

    thisNode.start();
    otherNode.start();

    thisNode.waitUntilStarted();
    otherNode.waitUntilStarted();

    auto &remoteSecondNode = thisNode.getModule<NodeNetworkModule>()->connectTo("127.0.0.1:9192");
    bool connectedToSecond = remoteSecondNode.isConnected();
    INFO("testing require")
    REQUIRE(connectedToSecond);
    std::this_thread::sleep_for(2s);
    bool hasNodeId = remoteSecondNode.getNodeId().has_value();
    REQUIRE(hasNodeId);
    auto realNodeId = *remoteSecondNode.getNodeId();
    REQUIRE(realNodeId == "second");

    //@todo test is broken, it hangs on queue transfer. fix queue function so it doesn't broadcast immediately, but only on start of the transfer.
//    //actual tests
    SECTION("transfer test") {
        fs::path testPath = fs::temp_directory_path() / "one";
        fs::path destinationPath = fs::temp_directory_path() / "two";
        INFO("test path is " << testPath.string())
        fs::create_directories(testPath);
        fs::create_directories(destinationPath);
        for (int i = 0; i < 50; ++i) {
            INFO("creating files for " + std::to_string(i))
            createFile(testPath / ("l" + std::to_string(i) + ".txt"), longString);
            createFile(testPath / ("v" + std::to_string(i) + ".txt"), veryLongString);
            createFile(testPath / ("e" + std::to_string(i) + ".txt"), extremelyLongString);
        }
        thisNode.setNodeContextActive();

        auto transferQueue = thisNode.getModule<FilesystemModule>()->transferQueue();

        for (int i = 0; i < 50; ++i) {
            INFO("downloading files for " + std::to_string(i))
            transferQueue->queueTransfer(std::make_shared<SimplePathRI>(testPath / ("l" + std::to_string(i) + ".txt")),
                                         std::make_shared<SimplePathRI>(
                                                 destinationPath / ("l" + std::to_string(i) + ".txt")));
            transferQueue->queueTransfer(std::make_shared<SimplePathRI>(testPath / ("v" + std::to_string(i) + ".txt")),
                                         std::make_shared<SimplePathRI>(
                                                 destinationPath / ("v" + std::to_string(i) + ".txt")));
            transferQueue->queueTransfer(std::make_shared<SimplePathRI>(testPath / ("e" + std::to_string(i) + ".txt")),
                                         std::make_shared<SimplePathRI>(
                                                 destinationPath / ("e" + std::to_string(i) + ".txt")));
        }

        transferQueue->start();
        transferQueue->waitToFinishAllTransfers();

        for (int i = 0; i < 50; ++i) {
            REQUIRE(readFile(destinationPath / ("l" + std::to_string(i) + ".txt")) == longString);
            REQUIRE(readFile(destinationPath / ("v" + std::to_string(i) + ".txt")) == veryLongString);
            REQUIRE(readFile(destinationPath / ("e" + std::to_string(i) + ".txt")) == extremelyLongString);
        }


    }


    //@todo add many large files on one node and transfer them to the other node.



    //finish and cleanup
    INFO("stopping")
    thisNode.stop();
    otherNode.stop();
    thisNode.waitToFinish();
    otherNode.waitToFinish();

}