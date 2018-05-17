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
#include <repo/journal/SimpleJournal.h>
#include <repo/node/RepoModule.h>
#include <p2p/network/node/modules/BasicModule.h>
#include <p2p/network/node/modules/FilesystemModule.h>
#include <p2p/dependency/Dependency.h>
#include <p2p/network/node/modules/NodeNetworkModule.h>
#include <p2p/network/protocol/role/Roles.h>
#include <p2p/network/node/modules/CommandModule.h>
#include <p2p/network/node/modules/command/StandardCommandsDirectory.h>
#include <p2p/filesystem/transfer/FileTransferControl.h>


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
    cmd->mapCommand("cd", &FilesystemModule::changeDirectory);
    cmd->mapCommand("pwd", &FilesystemModule::printWorkingDirectory);
    cmd->mapCommand("ls", &FilesystemModule::listCurrentDirectory);
//    cmd->mapCommand("scp", &FilesystemModule::remoteGetFile);
    cmd->mapCommand("scp", &FilesystemModule::remoteGetFile);
    cmd->mapCommand("lscp", &FilesystemModule::printCurrentTransfers);

    cmd->mapCommand("createRep", &RepoModule::createRepository);
    cmd->mapCommand("selectRep", &RepoModule::selectRepository);
    cmd->mapCommand("persist", &RepoModule::persistFile);
    cmd->mapCommand("saveRep", &RepoModule::saveRepository);
    cmd->mapCommand("loadRep", &RepoModule::loadRepository);
    cmd->mapCommand("printRep", &RepoModule::printHistory);
    cmd->mapCommand("downRep", &RepoModule::downloadRemoteRepository);
    cmd->mapCommand("resRep", &RepoModule::restoreRepository);
//    cmd->mapCommand("requestResource", &RepoModule::requestStoragePath);
    cmd->mapCommand("beginTransfer", &FilesystemModule::beginTransferTest);
//    cmd->submodule("help").mapCommand("cmdList",&CommandModule::listCommands);

}


#include <cereal/archives/xml.hpp>
#include <cereal/types/memory.hpp>
#include <repo/repository/storage/InternalStorage.h>
#include <p2p/utils/crypto.h>

struct PtrTest {
    int a = 9999;
    std::shared_ptr<PtrTest> ptr = nullptr;
private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(ptr), CEREAL_NVP(a));
    }


    friend class cereal::access;
};

//CEREAL_REGISTER_TYPE(PtrTest);

//class Const {
//public:
//
//    Const() {};
//    Const(const Const& c) {
//        LOGGER("copy const")
//    }
//
//    template<typename ... Args>
//    Const(Args... args) {
//        LOGGER("variadic")
//    }
//};




int main(int argc, char *argv[]) {

    std::shared_ptr<PtrTest> p1, p2, p3, p4, p5, p6, p7;

    p1 = std::make_shared<PtrTest>();
    p2 = std::make_shared<PtrTest>();
    p3 = std::make_shared<PtrTest>();
    p4 = std::make_shared<PtrTest>();
    p5 = std::make_shared<PtrTest>();

    p1->ptr = p2;
    p2->ptr = p3;
    p3->ptr = p1;
    p4->ptr = p1;
    p5->ptr = p1;
    p1->a = 1;
    p2->a = 2;
    p3->a = 3;
    p4->a = 4;
    p5->a = 5;


    {
        std::ofstream os("/tmp/ptrtest.xml");
        cereal::XMLOutputArchive oa(os);
        oa << p1;
        oa << p4;
        //  os << p5;
    }

    {
        std::ifstream is("/tmp/ptrtest.xml");
        cereal::XMLInputArchive ia(is);
        ia >> p5;
        ia >> p6;

        LOGGER(std::to_string(p5->ptr->ptr->a));
        LOGGER(std::to_string(p6->ptr->a));
    }

//    InternalStorage stor;
//    stor.initStorage();
//    stor.store(calculateSha1OfFile("/bin/zsh"),fs::file_size("/bin/zsh"),"/bin/zsh");
//    stor.restore(calculateSha1OfFile("/bin/zsh"),fs::file_size("/bin/zsh"),"/tmp/zsh");


    //  commandModule.runCommand(moduleName, commandName, arguments);

    //commandModule.mapCommandToModule<ModuleName, int, float, float>(moduleName, commandName, &ModuleName::function);

    RoleScope scope;
    scope.addRole("lala");

    scope.addRole("dupa");
    scope.addRole("test");
    Roles::setActiveScope(&scope);

    Roles::allowed("lala", "dupa", "test");
    //  exit(0);


    JournalGroup::Request::Ptr jgr = JournalGroup::Request::getNew();
    jgr->setRepoId("second");


    {
        std::ofstream os("/tmp/jgr.xml");
        cereal::XMLOutputArchive oa(os);
        oa << jgr;
        //  os << p5;
    }

    StaticUber<std::string> staticTest;
    staticTest.get<int>() = "lala int";
    staticTest.get<float>() = "lala float";
    staticTest.forEach([](std::string &s) { LOGGER(s); });

////exit(0);

    //LOGGER("dupa");

    ConfigurationManager cfg;

    auto ptrC = std::make_shared<IConfig>();
    cfg.save("dupa", ptrC);




//    Context context;
//    TransmissionControl transmissionControl;
//    context.setKey<std::string>(
//            "test", "wartosc");
//    std::clog << "Test context : " << *(context.get<std::string>("test")) << std::endl;
//    context.set<int>(0);
    Node thisNode(9191);
    thisNode.addToNetwork("TheNetwork");
    thisNode.getNodeInfo().setNodeId("first Node");

    setupModules(thisNode);

    auto cmdN = thisNode.getModule<CommandModule>();

    setupCommands(cmdN.get());
    cmdN->setInteractive(true);

    thisNode.start();
//    setupProtocolLogic(thisNode.getLogicManager(), transmissionControl);

    Node otherNode(9999);
    otherNode.getNodeInfo().setNodeId("second");
    otherNode.addToNetwork("TheNetwork");

    setupModules(otherNode);
    cmdN = otherNode.getModule<CommandModule>();
    setupCommands(cmdN.get());

    otherNode.start();
//    setupProtocolLogic(otherNode.getLogicManager(), transmissionControl);

    Node thirdNode(9898);
    thirdNode.getNodeInfo().setNodeId("third");
    thirdNode.addToNetwork("TheNetwork");
    setupModules(thirdNode);
    cmdN = thirdNode.getModule<CommandModule>();
    setupCommands(cmdN.get());

//    setupProtocolLogic(thirdNode.getLogicManager(), transmissionControl);
    thirdNode.start();


    thisNode.getNodeInfo().printAll();
    otherNode.getNodeInfo().printAll();
    thirdNode.getNodeInfo().printAll();
    thisNode.getModule<NodeNetworkModule>()->connectTo("127.0.0.1:9999");
    thisNode.getModule<NodeNetworkModule>()->updateNodeConnectionInfo();
    otherNode.getModule<NodeNetworkModule>()->updateNodeConnectionInfo();
    thirdNode.getModule<NodeNetworkModule>()->updateNodeConnectionInfo();

    std::this_thread::sleep_for(4s);
    //auto fdes = FileTransferControl::initiateTransfer(thisNode, "second", "/tmp/zsh", "/tmp/copied_zsh");
    thisNode.waitToFinish();
    otherNode.waitToFinish();
    thirdNode.waitToFinish();


//    LOGGER("connection was ... " + std::to_string(ret));
//    otherNode.getModule<NodeNetworkModule>()->connectTo("127.0.0.1:9898");
    //  thisNode.getModule<NodeNetworkModule>()->connectTo("127.0.0.1:100");
//    std::this_thread::sleep_for(5s);

//    thisNode.getModule<NodeNetworkModule>()->updateNodeConnectionInfo();
//    otherNode.getModule<NodeNetworkModule>()->updateNodeConnectionInfo();
//    thirdNode.getModule<NodeNetworkModule>()->updateNodeConnectionInfo();
//
//    thisNode.getModule<NodeNetworkModule>()->printConnections();
//    otherNode.getModule<NodeNetworkModule>()->printConnections();
//    thirdNode.getModule<NodeNetworkModule>()->printConnections();
//
//    std::this_thread::sleep_for(1s);
//    thisNode.getModule<NodeNetworkModule>()->purgeDuplicateConnections();
//    otherNode.getModule<NodeNetworkModule>()->purgeDuplicateConnections();
//    thirdNode.getModule<NodeNetworkModule>()->purgeDuplicateConnections();
//    thisNode.getModule<NodeNetworkModule>()->purgeInactiveConnections();
//    otherNode.getModule<NodeNetworkModule>()->purgeInactiveConnections();
//    thirdNode.getModule<NodeNetworkModule>()->purgeInactiveConnections();
//    //LOGGER("lala");
//    std::this_thread::sleep_for(2s);
//    thisNode.getModule<NodeNetworkModule>()->printConnections();
//    otherNode.getModule<NodeNetworkModule>()->printConnections();
//    thirdNode.getModule<NodeNetworkModule>()->printConnections();
//    std::this_thread::sleep_for(9995s);
//    LOGGER("stopping");
//    thisNode.stop();
//    otherNode.stop();
//    thirdNode.stop();
//    std::this_thread::sleep_for(5s);
//    thisNode.getModule<NodeNetworkModule>()->purgeInactiveConnections();
//    otherNode.getModule<NodeNetworkModule>()->purgeInactiveConnections();
//    thirdNode.getModule<NodeNetworkModule>()->purgeInactiveConnections();
//    std::this_thread::sleep_for(45s);
//    thisNode.getModule<NodeNetworkModule>()->printConnections();
//    otherNode.getModule<NodeNetworkModule>()->printConnections();
//    thirdNode.getModule<NodeNetworkModule>()->printConnections();
//    LOGGER("finishing");
//
//    std::this_thread::sleep_for(115s);
//    LOGGER("finished");

    return 0;
}
