#include <iostream>
#include <thread>

//#define CEREAL_THREAD_SAFE 1
#include "network/node/Node.h"
#include "logic/sources/ClockSource.h"
#include "network/protocol/logic/sources/AuthSource.h"
#include "network/protocol/logic/sources/ConnectionSource.h"
#include "network/protocol/logic/actions/TransmissionControl.h"
#include "network/protocol/logic/actions/ProtocolActions.h"
#include "network/node/protocol/logic/sources/NodeSource.h"
#include "network/node/protocol/logic/actions/NodeActions.h"
#include "network/node/protocol/logic/sources/NetworkSource.h"
#include "network/node/protocol/logic/actions/NetworkActions.h"


using namespace std::chrono_literals;

#include <fstream>


void setupProtocolLogic(LogicManager &logicManager, TransmissionControl &transmissionControl) {
    //adding sources
    logicManager.addSource<AuthSource>();
    logicManager.addSource<ClockSource>();
    logicManager.addSource<ConnectionSource>();
    logicManager.addSource<NetworkSource>();
    logicManager.addSource<NodeSource>();



    //setting actions

    logicManager.setAction<ConnectionEvent>("reqNoI", NodeActions::sendNodeInfoRequest);
    logicManager.setAction<ConnectionEvent>("reqNeI", NodeActions::sendNetworkInfoRequest);
    logicManager.setAction<NodeInfoEvent>("upNoI", NodeActions::updateNodeInfo);
    logicManager.setAction<NetworkInfoEvent>("upNeI", NetworkActions::updateNetworkInfo);
    logicManager.setAction<NodeInfoEvent>("addKnownNode", NodeActions::addKnownNode);
    logicManager.setAction<Tick>("trigNodeUp", NodeActions::triggerUpdateNode);
    logicManager.setAction<NodeInfoEvent>("nodeDiscovered", NodeActions::newNodeDiscovered);


    logicManager.setAction<ConnectionEvent>("connDebug", [](const ConnectionEvent &event) {

        // std::clog << "Debug: connection event!" << std::endl;
    });

    logicManager.setAction<PacketEvent>(PacketEventId::PACKET_RECEIVED,
                                        [&transmissionControl](
                                                               const PacketEvent &packetEvent) {
                                            return transmissionControl.onPacketReceived(packetEvent);
                                        });
    logicManager.setAction<PacketEvent>(PacketEventId::PACKET_SENT,
                                        [&transmissionControl](const PacketEvent &packetEvent) {
                                            return transmissionControl.onPacketSent(packetEvent);
    });

    logicManager.setAction<Tick>("TransTick", [&transmissionControl](const Tick &tick) {
        return transmissionControl.work(tick);
    });


    logicManager.setAction<ConnectionEvent>("onConnect", ProtocolActions::onNewConnection);

    //  assigning actions
    if (logicManager.assignAction<PacketEvent>(PacketEventId::PACKET_RECEIVED,
                                               PacketEventId::PACKET_RECEIVED)) {
        std::clog << "Debug: PACK RECV assignment!" << std::endl;
    }
    logicManager.assignAction<PacketEvent>(PacketEventId::PACKET_SENT, PacketEventId::PACKET_SENT);

    if (logicManager.assignAction<Tick>(500ms, "TransTick")) {
        std::clog << "Debug: Trans tick assignment!" << std::endl;

    }

    if (logicManager.assignAction<ConnectionEvent>("connDebug")) {
        std::clog << "Debug: ConEv assignment!" << std::endl;

    }


//    if (logicManager.assignAction<ConnectionEvent>("onConnect")) {
//        std::clog << "Debug: onConn assignment!" << std::endl;
//
//    }

    if (logicManager.assignAction<ConnectionEvent>("reqNoI")) {
        std::clog << "Debug: reqNoI assignment!" << std::endl;

    }

    if (logicManager.assignAction<ConnectionEvent>("reqNeI")) {
        std::clog << "Debug: reqNeI assignment!" << std::endl;

    }

    if (logicManager.assignAction<NodeInfoEvent>(NodeInfoEvent::IdType::NODE_INFO_RECEIVED, "upNoI")) {
        std::clog << "Debug: upNoI assignment!" << std::endl;

    }

    if (logicManager.assignAction<NetworkInfoEvent>(NetworkInfoEvent::IdType::NETWORK_INFO_RECEIVED, "upNeI")) {
        std::clog << "Debug: upNoI assignment!" << std::endl;

    }

    ///@todo upNoI powinno cos takiego robic, addKnownNode powinien byc wywolany tylko w przypadku
    if (logicManager.assignAction<NodeInfoEvent>(NodeInfoEvent::IdType::NODE_INFO_RECEIVED, "addKnownNode")) {
        std::clog << "Debug: addKnownNode assignment!" << std::endl;

    }

    if (logicManager.assignAction<NodeInfoEvent>(NodeInfoEvent::IdType::NEW_NODE_DISCOVERED, "nodeDiscovered")) {
        std::clog << "Debug: addKnownNode assignment!" << std::endl;

    }
//
//    if (logicManager.assignAction<Tick>(1500ms, "trigNodeUp")) {
//        std::clog << "Debug: TtrigNodeUp assignment!" << std::endl;
//
//    }


//
//    IProtocol* protocol = new DummyProtocol();
//    protocol->setupLogic(logicManager);

}


int main() {
//    auto ptr = NodeInfoGroup::Ack::getNew();
//        if (ptr->getStatus() == Status::ACK) {
//            LOGGER("STATUS OK");
//            exit(0);
//        }
//    NodeInfoGroup::Ack b1;
//    NodeInfoGroup::Response b2;
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
//  //  Base::Request::BaseType::Request x1;
//  //  std::cout << typeid(x1).name() << std::endl;
//    PacketInfo<Derived,Status::REQUEST>::Type x;
//    Base::Response y;
//    auto reqY = getType<Status::REQUEST>(y);
//    reqY.print();
//    auto resp = getType<Status::REQUEST>(x);//getResponse(x);
//    resp.print();
//    x.print();
//    //PacketInfo<Base,Status::RESPONSE>::Type a;

    LogicManager logicManager;
    logicManager.addSource<ClockSource>();
    //  logicManager.addSource<NetworkSource>();

    logicManager.setAction<Tick>("test", [](const Tick &t) {

        std::clog << "test tick " << t.getEventId().count() << std::endl;
    });


    LOGGER("dupa");


    Context context;
    TransmissionControl transmissionControl;
    context.setKey<std::string>(
            "test", "wartosc");
    std::clog << "Test context : " << *(context.get<std::string>("test")) << std::endl;
    context.set<int>(0);
    Node thisNode(9191);
    thisNode.addToNetwork("TheNetwork");
    thisNode.getNodeInfo().setNodeId("first Node");
    setupProtocolLogic(thisNode.getLogicManager(), transmissionControl);
    thisNode.start();

    Node otherNode(9999);
    otherNode.getNodeInfo().setNodeId("second node");
    otherNode.addToNetwork("TheNetwork");
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
    //    otherNode.connectTo("127.0.0.1:9898");
    // thisNode.connectTo("127.0.0.1:100");
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
    LOGGER("lala");
    std::this_thread::sleep_for(2s);
    thisNode.printConnections();
    otherNode.printConnections();
    thirdNode.printConnections();
    std::this_thread::sleep_for(15s);
    thirdNode.stop();
    otherNode.stop();
    thirdNode.stop();
    std::this_thread::sleep_for(5s);
    thisNode.purgeInactiveConnections();
//    thisNode.updateNodeConnectionInfo();
//    otherNode.updateNodeConnectionInfo();
//    thirdNode.updateNodeConnectionInfo();
    thisNode.printConnections();
    otherNode.printConnections();
    thirdNode.printConnections();
    return 0;
}
