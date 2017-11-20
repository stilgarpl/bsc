#include <iostream>
#include <thread>

//#define CEREAL_THREAD_SAFE 1
#include "p2p/network/node/Node.h"
#include "p2p/logic/sources/ClockSource.h"
#include "p2p/network/protocol/logic/sources/AuthSource.h"
#include "p2p/network/protocol/logic/sources/ConnectionSource.h"
#include "p2p/network/protocol/logic/actions/TransmissionControl.h"
#include "p2p/network/protocol/logic/actions/ProtocolActions.h"
#include "p2p/network/node/protocol/logic/sources/NodeSource.h"
#include "p2p/network/node/protocol/logic/actions/NodeActions.h"
#include "p2p/network/node/protocol/logic/sources/NetworkSource.h"
#include "p2p/network/node/protocol/logic/actions/NetworkActions.h"



using namespace std::chrono_literals;

#include <fstream>
#include <p2p/filesystem/network/logic/sources/FileSource.h>
#include <p2p/filesystem/network/logic/actions/FileActions.h>
#include <p2p/configuration/ConfigurationManager.h>
#include <repo/journal/Journal.h>
#include <repo/node/RepoModule.h>
#include <p2p/network/node/modules/BasicModule.h>
#include <p2p/network/node/modules/FilesystemModule.h>
#include <p2p/dependency/Dependency.h>
#include <p2p/network/node/modules/NodeNetworkModule.h>
#include <p2p/network/protocol/role/Roles.h>
#include <p2p/network/node/modules/CommandModule.h>
#include <p2p/network/node/modules/command/StandardCommandsDirectory.h>


void setupProtocolLogic(LogicManager &logicManager, TransmissionControl &transmissionControl) {
    //adding sources





    //setting actions







    //  assigning actions
    if (logicManager.assignAction<PacketEvent>(PacketEventId::PACKET_RECEIVED,
                                               PacketEventId::PACKET_RECEIVED)) {
        std::clog << "Debug: PACK RECV assignment!" << std::endl;
    }
    logicManager.assignAction<PacketEvent>(PacketEventId::PACKET_SENT, PacketEventId::PACKET_SENT);

    if (logicManager.assignAction<Tick>(500ms, "TransTick")) {
        std::clog << "Debug: Trans tick assignment!" << std::endl;

    }


    if (logicManager.assignAction<Tick>(1500ms, "trigNodeUp")) {
        std::clog << "Debug: TtrigNodeUp assignment!" << std::endl;

    }


    //both FILE and CHUNK events are assigned to the same function


//
//    IProtocol* protocol = new DummyProtocol();
//    protocol->setupLogic(logicManager);




}


void setupModules(Node &node) {
    node.addModule<BasicModule>();
    node.addModule<FilesystemModule>();
    node.addModule<NodeNetworkModule>();
    node.addModule<RepoModule>();
    node.addModule<CommandModule>();
}


void test(CommandModule::ArgumentContainerType arguments) {
    for (auto &&argument : arguments) {
        LOGGER(argument);
    }
}

void help(CommandModule::ArgumentContainerType arguments) {
    std::cout << "Help!" << std::endl;
}


void test1(std::string s) {

}


void setupCommands(CommandModule *cmd) {
    cmd->mapCommand("t2", &CommandModule::testingMethodInt);
    cmd->submodule("tt").mapCommand("t2", &CommandModule::testingMethodInt);
    cmd->submodule("tt").submodule("xx").mapCommand("tx", &CommandModule::testingMethodInt);
    cmd->mapCommand("t3", &CommandModule::testingMethodIntFloat);
    cmd->mapCommand("connect", &NodeNetworkModule::connectToAddress);
    cmd->mapCommand("disconnect", &NodeNetworkModule::disconnect);
    cmd->mapCommand("print", &NodeNetworkModule::printConnections);
    cmd->mapCommand("update", &NodeNetworkModule::updateNodeConnectionInfo);
    cmd->mapCommand("purgeD", &NodeNetworkModule::purgeDuplicateConnections);
    cmd->mapCommand("purgeI", &NodeNetworkModule::purgeInactiveConnections);
    cmd->mapRawCommand("remote", &CommandModule::sendRemoteCommand);
    cmd->mapCommand("shutdown", &BasicModule::shutdown);
}





int main(int argc, char *argv[]) {

//    DependencyManaged<int> ddd;`
//
//    ddd.addRequiredDependency<float,double,std::string>();
//    ddd.addRequiredDependency<float,float,long,long>();
//    exit(0);




    Node xnode;
    xnode.addModule<CommandModule>();
    auto cmdM = xnode.getModule<CommandModule>();

    cmdM->mapCommand("t2", &CommandModule::testingMethodInt);
    cmdM->mapCommand("t3", &CommandModule::testingMethodIntFloat);
    std::vector<std::string> arguments1;
    arguments1.push_back(std::string("7"));
    cmdM->runCommand("t2", arguments1);
    arguments1.push_back(std::string("2.3"));
    cmdM->runCommand("t3", arguments1);

    auto sub = cmdM->submodule("sub");
    sub.mapCommand("t4", &CommandModule::testingMethodInt);
    sub.runCommand("t4", {"99"});

    CommandModule commandModule(xnode);
    std::cout << std::flush;
    // exit(0);
//    commandModule.mapCommand( "do", &test);
//    commandModule.mapCommand("help", &help);

    commandModule.runCommand("xx", {std::string("lalala")});
    //  commandModule.runCommand("stuff", "do", {"test", "test2", "test3"});

    std::string moduleName = "help";
    std::string commandName = "help";
    std::vector<std::string> arguments;

    if (argc > 1) {
        moduleName = argv[1];
        if (argc > 2) {
            commandName = argv[2];
            arguments.resize(static_cast<unsigned long>(argc - 2));
            for (int i = 3; i < argc; ++i) {
                arguments[i - 3] = argv[i];
            }
        }
    }



//    runMemberFunction(&TempTest::test, arguments1);
//    runMemberFunction(&TempTest::test, arguments);
//    runMemberFunction(&TempTest::test, arguments1);

//
//    LOGGER("pre")
//    test(arguments);
//    LOGGER("Post")

    //  commandModule.runCommand(moduleName, commandName, arguments);

    //commandModule.mapCommandToModule<ModuleName, int, float, float>(moduleName, commandName, &ModuleName::function);

    RoleScope scope;
    scope.addRole("lala");

    scope.addRole("dupa");
    scope.addRole("test");
    Roles::setActiveScope(&scope);

    Roles::allowed("lala", "dupa", "test");
    //  exit(0);



    StaticUber<std::string> staticTest;
    staticTest.get<int>() = "lala int";
    staticTest.get<float>() = "lala float";
    staticTest.forEach([](std::string &s) { LOGGER(s); });

    Journal journal;

    journal.append(JournalMethod::ADDED, "/tmp/dupa.txt");
    journal.append(JournalMethod::DELETED, "/tmp/to_remove.txt");
    journal.commitState();
    journal.append(JournalMethod::UPDATED, "/tmp/dupa.txt");
    journal.replay();
    journal.commitState();

    {
        std::ofstream os("/tmp/journal.xml");
        cereal::XMLOutputArchive oa(os);
        oa << journal;
    }

    LOGGER(journal.calculateChecksum());
    LOGGER(journal.calculateChecksum());
    LOGGER(journal.calculateChecksum());
    //  exit(0);
//    SendFile::Response sfRes;
//    fs::path tmpPath = "/tmp/basyco/testfile.txt";
//    fs::path tmpPath2 = "/tmp/basyco/testfile2.txt";
//
//    sfRes.load_file(tmpPath);
//    sfRes.setFilePath(tmpPath2);
//    sfRes.save_file();
////exit(0);

    //LOGGER("dupa");

    ConfigurationManager cfg;

    auto ptrC = std::make_shared<IConfig>();
    cfg.save("dupa", ptrC);




    Context context;
    TransmissionControl transmissionControl;
    context.setKey<std::string>(
            "test", "wartosc");
    std::clog << "Test context : " << *(context.get<std::string>("test")) << std::endl;
    context.set<int>(0);
    Node thisNode(9191);
    thisNode.addToNetwork("TheNetwork");
    thisNode.getNodeInfo().setNodeId("first Node");

    setupModules(thisNode);

    auto cmdN = thisNode.getModule<CommandModule>();

    setupCommands(cmdN.get());
    cmdN->setInteractive(true);

    thisNode.start();
    setupProtocolLogic(thisNode.getLogicManager(), transmissionControl);

    Node otherNode(9999);
    otherNode.getNodeInfo().setNodeId("second");
    otherNode.addToNetwork("TheNetwork");

    setupModules(otherNode);
    cmdN = otherNode.getModule<CommandModule>();
    setupCommands(cmdN.get());

    otherNode.start();
    setupProtocolLogic(otherNode.getLogicManager(), transmissionControl);

    Node thirdNode(9898);
    thirdNode.getNodeInfo().setNodeId("third");
    thirdNode.addToNetwork("TheNetwork");
    setupModules(thirdNode);
    cmdN = thirdNode.getModule<CommandModule>();
    setupCommands(cmdN.get());

    setupProtocolLogic(thirdNode.getLogicManager(), transmissionControl);
    thirdNode.start();


    thisNode.getNodeInfo().printAll();
    otherNode.getNodeInfo().printAll();
    thirdNode.getNodeInfo().printAll();
    std::this_thread::sleep_for(2s);
    bool ret = thisNode.getModule<NodeNetworkModule>()->connectTo("127.0.0.1:9999");
    LOGGER("connection was ... " + std::to_string(ret));
    otherNode.getModule<NodeNetworkModule>()->connectTo("127.0.0.1:9898");
    //  thisNode.getModule<NodeNetworkModule>()->connectTo("127.0.0.1:100");
    std::this_thread::sleep_for(5s);

    thisNode.getModule<NodeNetworkModule>()->updateNodeConnectionInfo();
    otherNode.getModule<NodeNetworkModule>()->updateNodeConnectionInfo();
    thirdNode.getModule<NodeNetworkModule>()->updateNodeConnectionInfo();

    thisNode.getModule<NodeNetworkModule>()->printConnections();
    otherNode.getModule<NodeNetworkModule>()->printConnections();
    thirdNode.getModule<NodeNetworkModule>()->printConnections();

    std::this_thread::sleep_for(1s);
    thisNode.getModule<NodeNetworkModule>()->purgeDuplicateConnections();
    otherNode.getModule<NodeNetworkModule>()->purgeDuplicateConnections();
    thirdNode.getModule<NodeNetworkModule>()->purgeDuplicateConnections();
    thisNode.getModule<NodeNetworkModule>()->purgeInactiveConnections();
    otherNode.getModule<NodeNetworkModule>()->purgeInactiveConnections();
    thirdNode.getModule<NodeNetworkModule>()->purgeInactiveConnections();
    //LOGGER("lala");
    std::this_thread::sleep_for(2s);
    thisNode.getModule<NodeNetworkModule>()->printConnections();
    otherNode.getModule<NodeNetworkModule>()->printConnections();
    thirdNode.getModule<NodeNetworkModule>()->printConnections();
    std::this_thread::sleep_for(9995s);
    LOGGER("stopping");
    thisNode.stop();
    otherNode.stop();
    thirdNode.stop();
    std::this_thread::sleep_for(5s);
    thisNode.getModule<NodeNetworkModule>()->purgeInactiveConnections();
    otherNode.getModule<NodeNetworkModule>()->purgeInactiveConnections();
    thirdNode.getModule<NodeNetworkModule>()->purgeInactiveConnections();
    std::this_thread::sleep_for(45s);
    thisNode.getModule<NodeNetworkModule>()->printConnections();
    otherNode.getModule<NodeNetworkModule>()->printConnections();
    thirdNode.getModule<NodeNetworkModule>()->printConnections();
    LOGGER("finishing");

    std::this_thread::sleep_for(115s);
    LOGGER("finished");

    return 0;
}
