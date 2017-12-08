//
// Created by stilgar on 08.12.17.
//

#include <catch/catch.hpp>
#include <p2p/network/node/modules/CommandModule.h>
#include <repo/node/RepoModule.h>
#include <p2p/network/node/modules/FilesystemModule.h>
#include <p2p/network/node/modules/BasicModule.h>

void setupModules(Node &node) {
    node.addModule<BasicModule>();
    node.addModule<FilesystemModule>();
    node.addModule<NodeNetworkModule>();
    node.addModule<RepoModule>();
    node.addModule<CommandModule>();
}


TEST_CASE("Repo module test", "[!throws]") {

    Node thisNode(9191);
    thisNode.addToNetwork("TheNetwork");
    thisNode.getNodeInfo().setNodeId("first Node");

    setupModules(thisNode);

    auto cmdN = thisNode.getModule<CommandModule>();


    Node otherNode(9999);
    otherNode.getNodeInfo().setNodeId("second");
    otherNode.addToNetwork("TheNetwork");

    setupModules(otherNode);


    thisNode.start();
    otherNode.start();

    thisNode.getModule<NodeNetworkModule>()->connectTo("127.0.0.1:9999");
    thisNode.getModule<NodeNetworkModule>()->updateNodeConnectionInfo();
    bool connectedToSecond = std::find_if(thisNode.getModule<NodeNetworkModule>()->getClientConnections().begin(),
                                          thisNode.getModule<NodeNetworkModule>()->getClientConnections().end(),
                                          [&](NodeConnectionInfoPtr i) {
                                              return i->connection->isActive() && i->nodeId && *i->nodeId == "second";
                                          }) != thisNode.getModule<NodeNetworkModule>()->getClientConnections().end();
    REQUIRE(connectedToSecond);

    SECTION ("create repository on second ") {
        auto otherRepoMod = otherNode.getModule<RepoModule>();
        REQUIRE(otherRepoMod != nullptr);
        otherRepoMod->createRepository("test");
        REQUIRE(otherRepoMod->findRepository("test") != nullptr);
        otherRepoMod->selectRepository("test");
        REQUIRE(otherRepoMod->getSelectedRepository() != nullptr);
        otherRepoMod->persistFile("/etc/hosts");
        otherRepoMod->saveRepository("test");
        REQUIRE(fs::exists("/tmp/test.xml"));

        auto otherSum = otherRepoMod->getSelectedRepository()->getJournal()->getChecksum();


        SECTION("download remote repository") {

            auto thisRepoMod = thisNode.getModule<RepoModule>();

            REQUIRE_FALSE(thisRepoMod->findRepository("test") != nullptr);

            thisRepoMod->downloadRemoteRepository("second", "test");
            REQUIRE(thisRepoMod->findRepository("test") != nullptr);

            thisRepoMod->selectRepository("test");
            REQUIRE(thisRepoMod->getSelectedRepository() != nullptr);

            auto thisSum = thisRepoMod->getSelectedRepository()->getJournal()->getChecksum();
            REQUIRE(thisSum == otherSum);


        }
    }

//    WARN("closing");
    thisNode.stop();
    otherNode.stop();
    ///@todo stop() should join all threads, including poco ones. stopping the app before it has finished crashes the app
    std::this_thread::sleep_for(3s);


}