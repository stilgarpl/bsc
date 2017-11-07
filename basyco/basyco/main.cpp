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
    node.addModule<RepoModule>();
}


int main() {


    Dependency<int, float, int, unsigned int, unsigned long, long, signed long, float, int, unsigned int, unsigned long, long, signed long, float, int, unsigned int, unsigned long, long, signed long, float, int, unsigned int, unsigned long, long, signed long, float, int, unsigned int, unsigned long, long, signed long, float, int, unsigned int, unsigned long, long, signed long, float, int, unsigned int, unsigned long, long, signed long, float, int, unsigned int, unsigned long, long, signed long> dep;
    auto before = std::chrono::steady_clock::now();
    auto val = dep.getDependencyIds();
//     val = dep.getIds();
//     val = dep.getIds();
//     val = dep.getIds();
    auto after = std::chrono::steady_clock::now();


    for (auto &&item : val) {
        std::cout << "Val : " << item << std::endl;
    }


    DependencyManager::DependencyList depList;
    auto intD = std::make_shared<DependencyManaged<int>>();
    auto floatD = std::make_shared<DependencyManaged<float>>();
    auto doubleD = std::make_shared<DependencyManaged<double>>();
    intD->setRequired(std::make_shared<Dependency1>());
    floatD->setRequired(std::make_shared<Dependency<int>>());
    doubleD->setRequired(std::make_shared<Dependency<int, float>>());


    depList.push_back(doubleD);
    depList.push_back(intD);
    depList.push_back(floatD);

    for (auto &&list : depList) {
        std::cout << "edek : " << list->getDepedencyId() << std::endl;

    }

    auto redep = DependencyManager::dependencySort(depList);
    for (auto &&list : redep) {
        std::cout << "fedek : " << list->getDepedencyId() << std::endl;

    }
    exit(0);



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
