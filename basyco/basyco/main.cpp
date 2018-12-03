#include <iostream>
#include <thread>

//#define CEREAL_THREAD_SAFE 1
#include "p2p/node/Node.h"
#include "p2p/logic/sources/ClockSource.h"
#include "p2p/modules/nodeNetworkModule/protocol/logic/sources/ConnectionSource.h"
#include "p2p/modules/nodeNetworkModule/protocol/logic/actions/TransmissionControl.h"
#include "p2p/modules/nodeNetworkModule/protocol/logic/actions/ProtocolActions.h"
#include "p2p/modules/nodeNetworkModule/protocol/logic/sources/NodeSource.h"
#include "p2p/modules/nodeNetworkModule/protocol/logic/actions/NodeActions.h"
#include "p2p/modules/nodeNetworkModule/protocol/logic/sources/NetworkSource.h"
#include "p2p/modules/nodeNetworkModule/protocol/logic/actions/NetworkActions.h"



using namespace std::chrono_literals;

#include <repo/journal/SimpleJournal.h>
#include <repo/node/RepoModule.h>
#include <p2p/modules/filesystem/FilesystemModule.h>
#include <p2p/dependency/Dependency.h>
#include <p2p/modules/command/CommandModule.h>
#include <p2p/modules/command/StandardCommandsDirectory.h>
#include <variant>

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
    node.addModule<ConfigurationModule>();
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
    cmd->mapCommand("shutdown", &BasicModule::shutdown);
    cmd->mapCommand("cd", &FilesystemModule::changeDirectory);
    cmd->mapCommand("pwd", &FilesystemModule::printWorkingDirectory);
    cmd->mapCommand("ls", &FilesystemModule::listCurrentDirectory);
//    cmd->mapCommand("scp", &FilesystemModule::remoteGetFile);
    cmd->mapCommand("scp", &FilesystemModule::remoteGetFile);
//    cmd->mapCommand("lscp", &FilesystemModule::printCurrentTransfers);

    cmd->mapCommand("createRep", &RepoModule::createRepository);
    cmd->mapCommand("selectRep", &RepoModule::selectRepository);
    cmd->mapCommand("persist", &RepoModule::persistFile);
    cmd->mapCommand("saveRep", &RepoModule::saveRepository);
    cmd->mapCommand("loadRep", &RepoModule::loadRepository);
    cmd->mapCommand("printRep", &RepoModule::printHistory);
    cmd->mapCommand("downRep", &RepoModule::downloadRemoteRepository);
    cmd->mapCommand("resRep", &RepoModule::restoreRepository);
    cmd->mapCommand("update", &RepoModule::updateFile);
    cmd->mapCommand("ignore", &RepoModule::ignoreFile);
    cmd->mapCommand("updateAll", &RepoModule::updateAllFiles);
//    cmd->mapCommand("requestResource", &RepoModule::requestStoragePath);
//    cmd->mapCommand("beginTransfer", &FilesystemModule::beginTransferTest);
//    cmd->submodule("help").mapCommand("cmdList",&CommandModule::listCommands);

}


#include <cereal/archives/xml.hpp>
#include <cereal/types/memory.hpp>
#include <repo/repository/storage/InternalStorage.h>
#include <p2p/modules/nodeNetworkModule/protocol/packet/NodeInfoGroup.h>

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

//class outbuf : public std::streambuf
//{
//protected:
//    /* central output function
//     * - print characters in uppercase mode
//     */
//    virtual int_type overflow (int_type c) {
//        if (c != EOF) {
//            // convert lowercase to uppercase
//            c = std::toupper(static_cast<char>(c),getloc());
//
//            // and write the character to the standard output
//            if (putchar(c) == EOF) {
//                return EOF;
//            }
//        }
//        return c;
//    }
//};

class InputBuffer : public std::streambuf {
    std::string buffer;
    std::streambuf &source;

public:
    explicit InputBuffer(std::streambuf &d) : source(d) {}


protected:
    int underflow() override {
        int result(EOF);
        if (gptr() < egptr())
            result = *gptr();
        else {
            long value = 0;
            for (unsigned int i = 0; i < sizeof(buffer.size()); ++i) {
                value += source.sbumpc() << i * 8;
                auto c = static_cast<char>((buffer.size() & 0xFF << i * 8) >> i * 8);
            }
            LOGGER("value read is " + std::to_string(value));
            //now read n bytes from the source
            for (int j = 0; j < value; ++j) {
                buffer += source.sbumpc();
            }
            setg(buffer.begin().base(), buffer.begin().base(), buffer.end().base());
            return static_cast<int>(value);
        }
        return result;
    }

};

class OutputBuffer : public std::streambuf {
private:
    std::string buffer;
    std::ostream &dst;
public:

    explicit OutputBuffer(std::ostream &dst) : dst(dst) {}

protected:
    int overflow(int_type c) override {
//        LOGGER( + (unsigned char)c);
        std::clog << "overflow: " << (char) c << std::endl;
        if (c != EOF) {
            buffer += (char) c;
        } else {
            flush();
        }
        return c;
    }

public:
    void flush() {
        //@todo the size of this parameter should be constant, so no sizeof but 4 or 8 bytes ALWAYS
        for (unsigned int i = 0; i < sizeof(buffer.size()); ++i) {
            auto c = static_cast<char>((buffer.size() & 0xFF << i * 8) >> i * 8);
            dst << c;

        }
        dst << buffer;
        buffer.clear();
        //sputn(result.c_str(),result.size());
//        dst << result;
    }
};


class LogicTest : public LogicStateMachine<LogicTest, int> {

public:
    LogicTest() : LogicStateMachine<LogicTest, int>(*this) {
        addState(1, 2, 3, 4, 5, 6);
        addLink(1, 2, 3);
        addLink(2, 4, 5);
    }

};

class LogicTestObserver : public LogicTest::Observer {
public:
    void update(LogicTest &object, int type) override {
        LOGGER("update for " + std::to_string(type));
    }
};



int main(int argc, char *argv[]) {
//    {

//    LogicManager logicManager;
//Context::Ptr context = Context::makeContext();
//    context->set<LogicContext, LogicManager &>(logicManager);
//    Context::setActiveContext(context);
//    logicManager.start();
//
//LogicTest logicTest;
//
//LogicTestObserver observer;
//
//logicTest.registerStateObserver(observer);
//logicTest.setState(1);
//logicTest.changeState(2);
//    logicTest.changeState(4);
//    logicTest.changeState(1);
//    logicTest.unregisterStateObserver(observer);


//exit(0);




//        // create special output buffer
////        OutputBuffer ob(std::cout);
//        std::stringstream ss;
////
////        ss << 0x1 << "test" << "dupa";
////        std::string x;
////        ss >> x;
////        std::cout << x ;
//        OutputBuffer ob(ss);
//        // initialize output stream with that output buffer
//
//        std::ostream out(&ob);
////std::cout << "what?";
//        out << "test of test";
//        out << "aaaa";
//        ob.flush();
//        out << "bb";
//        out.flush();
//        ob.flush();
//        std::string s1 = ss.str();
//        auto cs = s1.c_str();
//        std::cout << cs << std::endl;
//
//        InputBuffer ib(*ss.rdbuf());
//        std::istream iss(&ib);
//        std::string x1,x2;
//        iss >> x1;
//        iss >> x2;
//        iss >> x2;
//        iss >> x2;
//
//        std::cout << " X1 : " << x1 << "X2: " << x2 << std::endl;
//    }
//
//    ///cereal test
//
//    std::stringstream io;
//    cereal::BinaryOutputArchive oa(io);
//    NodeInfoGroup::Request::Ptr nodeReq = NodeInfoGroup::Request::getNew();
//    NetworkInfoRequest::Ptr netReq = NetworkInfoRequest::getNew();
//
//    oa << nodeReq;
//    oa << netReq;
//    NodeInfoGroup::Request::Ptr nodeReq2;
//    NetworkInfoRequest::Ptr netReq2;
//
//    cereal::BinaryInputArchive ia(io);
//
//    ia >>nodeReq2;
//    ia >> netReq2;
//
//    LOGGER(std::string("nR2 : ") + typeid(nodeReq2).name() + "netR2 " + typeid(netReq2).name());
//
//
//
//    return 0;

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

//    ConfigurationManager cfg;
//
//    auto ptrC = std::make_shared<IConfig>();
//    cfg.save("dupa", ptrC);


//    Context context;
//    TransmissionControl transmissionControl;
//    context.setKey<std::string>(
//            "test", "wartosc");
//    std::clog << "Test context : " << *(context->get<std::string>("test")) << std::endl;
//    context.set<int>(0);
    Node thisNode(9191);

    thisNode.getNodeInfo().setNodeId("first Node");

    setupModules(thisNode);
    thisNode.getModule<NodeNetworkModule>()->addToNetwork("TheNetwork");
    thisNode.getModule<NodeNetworkModule>()->configuration().setPort(9191);

    auto cmdN = thisNode.getModule<CommandModule>();

    setupCommands(cmdN.get());
    cmdN->setInteractive(true);

    thisNode.start();
//    setupProtocolLogic(thisNode.getLogicManager(), transmissionControl);

    Node otherNode(9999);
    otherNode.getNodeInfo().setNodeId("second");


    setupModules(otherNode);
    otherNode.getModule<NodeNetworkModule>()->addToNetwork("TheNetwork");
    otherNode.getModule<NodeNetworkModule>()->configuration().setPort(9999);
    cmdN = otherNode.getModule<CommandModule>();
    setupCommands(cmdN.get());

    otherNode.start();
//    setupProtocolLogic(otherNode.getLogicManager(), transmissionControl);

    Node thirdNode(9898);
    thirdNode.getNodeInfo().setNodeId("third");

    setupModules(thirdNode);
    thirdNode.getModule<NodeNetworkModule>()->addToNetwork("TheNetwork");
    thirdNode.getModule<NodeNetworkModule>()->configuration().setPort(9898);
    cmdN = thirdNode.getModule<CommandModule>();
    setupCommands(cmdN.get());

//    setupProtocolLogic(thirdNode.getLogicManager(), transmissionControl);
    thirdNode.start();

    //@todo this is required so logic is set up before connecting. change it so start wait until the end of the initialization phase
    // std::this_thread::sleep_for(1000ms);

    thisNode.getNodeInfo().printAll();
    otherNode.getNodeInfo().printAll();
    thirdNode.getNodeInfo().printAll();
    thisNode.getModule<NodeNetworkModule>()->connectTo("127.0.0.1:9999");
//    thisNode.getModule<NodeNetworkModule>()->connectTo("127.0.0.1:9898");
//    thisNode.getModule<NodeNetworkModule>()->updateNodeConnectionInfo();
//    otherNode.getModule<NodeNetworkModule>()->updateNodeConnectionInfo();
//    thirdNode.getModule<NodeNetworkModule>()->updateNodeConnectionInfo();

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
