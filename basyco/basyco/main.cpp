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


void setupProtocolLogic(LogicManager &logicManager, TransmissionControl &transmissionControl) {
    //adding sources





    //setting actions







//    //  assigning actions
//    if (logicManager.assignAction<PacketEvent>(PacketEventId::PACKET_RECEIVED,
//                                               PacketEventId::PACKET_RECEIVED)) {
//        std::clog << "Debug: PACK RECV assignment!" << std::endl;
//    }
//    logicManager.assignAction<PacketEvent>(PacketEventId::PACKET_SENT, PacketEventId::PACKET_SENT);
//
//    if (logicManager.assignAction<Tick>(500ms, "TransTick")) {
//        std::clog << "Debug: Trans tick assignment!" << std::endl;
//
//    }



//    if (logicManager.assignAction<Tick>(1500ms, "trigNodeUp")) {
//        std::clog << "Debug: TtrigNodeUp assignment!" << std::endl;
//
//    }


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
}


void test(CommandModule::ArgumentContainerType arguments) {
    for (auto &&argument : arguments) {
        //LOGGER(argument);
    }
}

void help(CommandModule::ArgumentContainerType arguments) {
    std::cout << "Help!" << std::endl;
}


void test1(std::string s) {

}


template<std::size_t... Indices>
struct indices {
    using next = indices<Indices..., sizeof...(Indices)>;
};
template<std::size_t N>
struct build_indices {
    using type = typename build_indices<N - 1>::type::next;
};
template<>
struct build_indices<0> {
    using type = indices<>;
};
template<std::size_t N>
using BuildIndices = typename build_indices<N>::type;

template<typename Iterator>
using ValueType = typename std::iterator_traits<Iterator>::value_type;


template<typename T, typename S>
T from_string(const S &s) {
    return T(s);
}

template<>
int from_string<int>(const std::string &s) {
    return std::atoi(s.c_str());
}


template<typename ... Strings>
struct RunCastString {
    template<typename T, typename ...Args>
    void cast_all(T &t, void(T::*fun)(Args...), Strings...as) {
        (t.*fun)(from_string<Args>(as)...);
    }
};


template<typename T, typename RetType, typename ... Args, typename ... Strings>
static void runStringCast(T &t, RetType (T::*f)(Args...), Strings ... strings) {
    RunCastString<Strings...> r;
    r.cast_all(t, f, strings...);
};


template<size_t num_args>
struct unpack_caller {
private:
    template<typename T, typename RetType, typename ... Args, size_t... I>
    void call(T &t, RetType(T::*f)(Args...), std::vector<std::string> &args, indices<I...>) {
        runStringCast(t, f, args[I]...);
    }

    template<typename RetType, typename ... Args, size_t... I>
    void call(RetType(*f)(Args...), std::vector<std::string> &args, indices<I...>) {
        (*f)(args[I]...);
    }


public:
    template<typename T, typename RetType, typename ... Args>
    void operator()(T &t, RetType(T::*f)(Args...), std::vector<std::string> &args) {
        assert(args.size() == num_args); // just to be sure
        call(t, f, args, BuildIndices<num_args>{});
    }

    template<typename RetType, typename ... Args>
    void operator()(RetType(*f)(Args...), std::vector<std::string> &args) {
        //assert(args.size() == num_args); // just to be sure
        call(f, args, BuildIndices<num_args>{});
    }
};


template<typename T, typename RetType, typename ... Args>
void getMemFun(T &t, RetType (T::*f)(Args...), std::vector<std::string> values) {
    unpack_caller<sizeof... (Args)> a;
    a(t, f, values);
};


template<typename ModuleType, typename T, typename RetType, typename ... Args>
void mapFuncToModule(Node &node, std::string moduleName, std::string commandName, RetType (T::*f)(Args...)) {
    auto mod = node.getModule<ModuleType>();
    auto command = node.getModule<CommandModule>();
    command->mapCommand(moduleName, commandName, [=](CommandModule::ArgumentContainerType vals) {
        getMemFun(*mod, f, vals);
    });
};






int main(int argc, char *argv[]) {


    std::string sss("lala");


    Node xnode;
    xnode.addModule<CommandModule>();
    auto cmdM = xnode.getModule<CommandModule>();

    mapFuncToModule<CommandModule>(xnode, "t1", "t2", &CommandModule::testingMethodInt);

    std::vector<std::string> arguments1;
    arguments1.push_back(std::string("gggg18675"));

    cmdM->runCommand("t1", "t2", arguments1);
    CommandModule commandModule(xnode);

    commandModule.mapCommand("stuff", "do", &test);
    commandModule.mapCommand("help", "help", &help);

    commandModule.runCommand("dupa", "xx", {std::string("lalala")});
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



//    getMemFun(&TempTest::test, arguments1);
//    getMemFun(&TempTest::test, arguments);
//    getMemFun(&TempTest::test, arguments1);

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
    exit(0);
//    Dependency<int, float, int, unsigned int, unsigned long, long, signed long, float, int, unsigned int, unsigned long, long, signed long, float, int, unsigned int, unsigned long, long, signed long, float, int, unsigned int, unsigned long, long, signed long, float, int, unsigned int, unsigned long, long, signed long, float, int, unsigned int, unsigned long, long, signed long, float, int, unsigned int, unsigned long, long, signed long, float, int, unsigned int, unsigned long, long, signed long> dep;
//    auto before = std::chrono::steady_clock::now();
//    auto val = dep.getDependencyIds();
////     val = dep.getIds();
////     val = dep.getIds();
////     val = dep.getIds();
//    auto after = std::chrono::steady_clock::now();
//
//
//    for (auto &&item : val) {
//        std::cout << "Val : " << item << std::endl;
//    }
//
//
//    DependencyManager::DependencyList depList;
//    auto intD = std::make_shared<DependencyManaged<int>>();
//    auto floatD = std::make_shared<DependencyManaged<float>>();
//    auto doubleD = std::make_shared<DependencyManaged<double>>();
//    intD->setRequired(std::make_shared<Dependency1>());
//    floatD->setRequired(std::make_shared<Dependency<int>>());
//    doubleD->setRequired(std::make_shared<Dependency<int, float>>());
//
//
//    depList.push_back(doubleD);
//    depList.push_back(intD);
//    depList.push_back(floatD);
//
//    for (auto &&list : depList) {
//        std::cout << "edek : " << list->getDepedencyId() << std::endl;
//
//    }
//
//    auto redep = DependencyManager::dependencySort(depList);
//    for (auto &&list : redep) {
//        std::cout << "fedek : " << list->getDepedencyId() << std::endl;
//
//    }
//    exit(0);



//    auto ptr = NodeInfoGroup::Ack::getNew();
//        if (ptr->getStatus() == Status::ACK) {
//            LOGGER("STATUS OK");
//            exit(0);
//        }
//    NodeInfoGroup::Ack b1;
//    NodeInfoGroup::SendFileResponse b2;
//    auto status = NodeInfoGroup::Ack::getPacketStatus();
//
//    exit(0);
//    BasePacket basePacket;
//    PacketUtils::getNew<Status::RESPONSE>(basePacket);
//
//    NodeInfoRequest nReq;
//    auto nRes = PacketUtils::getNew<Status::RESPONSE>(nReq);
//    nRes.setId(5);
//    nRes.getNodeInfo().printAll();
//  //  Base::SendFileRequest::BaseType::SendFileRequest x1;
//  //  std::cout << typeid(x1).name() << std::endl;
//    PacketInfo<Derived,Status::REQUEST>::Type x;
//    Base::SendFileResponse y;
//    auto reqY = getType<Status::REQUEST>(y);
//    reqY.print();
//    auto resp = getType<Status::REQUEST>(x);//getResponse(x);
//    resp.print();
//    x.print();
//    //PacketInfo<Base,Status::RESPONSE>::Type a;



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
    setupProtocolLogic(thisNode.getLogicManager(), transmissionControl);
    thisNode.start();

    Node otherNode(9999);
    otherNode.getNodeInfo().setNodeId("second node");
    otherNode.addToNetwork("TheNetwork");

    setupModules(otherNode);
    setupProtocolLogic(otherNode.getLogicManager(), transmissionControl);
    otherNode.start();

    Node thirdNode(9898);
    thirdNode.getNodeInfo().setNodeId("third node");
    thirdNode.addToNetwork("TheNetwork");
    setupProtocolLogic(thirdNode.getLogicManager(), transmissionControl);
    //   thirdNode.start();


    thisNode.getNodeInfo().printAll();
    otherNode.getNodeInfo().printAll();
    thirdNode.getNodeInfo().printAll();
    thisNode.connectTo("127.0.0.1:9999");
    otherNode.connectTo("127.0.0.1:9898");
    thisNode.connectTo("127.0.0.1:100");
    std::this_thread::sleep_for(5s);

    thisNode.updateNodeConnectionInfo();
    otherNode.updateNodeConnectionInfo();
    thirdNode.updateNodeConnectionInfo();

    thisNode.printConnections();
    otherNode.printConnections();
    thirdNode.printConnections();

    std::this_thread::sleep_for(1s);
    thisNode.purgeDuplicateConnections();
    otherNode.purgeDuplicateConnections();
    thirdNode.purgeDuplicateConnections();
    thisNode.purgeInactiveConnections();
    otherNode.purgeInactiveConnections();
    thirdNode.purgeInactiveConnections();
    //LOGGER("lala");
    std::this_thread::sleep_for(2s);
    thisNode.printConnections();
    otherNode.printConnections();
    thirdNode.printConnections();
    std::this_thread::sleep_for(5s);
    LOGGER("stopping");
    thisNode.stop();
    otherNode.stop();
    thirdNode.stop();
    std::this_thread::sleep_for(5s);
    thisNode.purgeInactiveConnections();
    otherNode.purgeInactiveConnections();
    thirdNode.purgeInactiveConnections();
    thisNode.printConnections();
    otherNode.printConnections();
    thirdNode.printConnections();
    LOGGER("finishing");

    std::this_thread::sleep_for(5s);
    LOGGER("finished");

    return 0;
}
