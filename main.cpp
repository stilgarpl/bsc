#include <iostream>
#include <thread>

//#define CEREAL_THREAD_SAFE 1
#include "network/node/Node.h"
#include "logic/sources/ClockSource.h"
#include "network/logic/sources/NetworkSource.h"
#include "network/protocol/logic/sources/AuthSource.h"
#include "network/protocol/logic/sources/ConnectionSource.h"
#include "network/protocol/logic/actions/TransmissionControl.h"
#include "network/protocol/logic/actions/ProtocolActions.h"
#include "network/node/protocol/logic/sources/NodeSource.h"
#include "network/node/protocol/logic/actions/NodeActions.h"


using namespace std::chrono_literals;

#include <fstream>


class RefX {
    int &a;
public:
    int &getA() const { return a; };

    RefX(int &a) : a(a) {};
};


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
    logicManager.setAction<NodeEvent>("upNoI", NodeActions::updateNodeInfo);
    logicManager.setAction<NodeEvent>("upNeI", NodeActions::updateNetworkInfo);
    logicManager.setAction<NodeEvent>("addKnownNode", NodeActions::addKnownNode);
    logicManager.setAction<Tick>("trigNodeUp", NodeActions::triggerUpdateNode);
    logicManager.setAction<NodeEvent>("nodeDiscovered", NodeActions::newNodeDiscovered);


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

    //assigning actions
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

    if (logicManager.assignAction<NodeEvent>(NodeEvent::IdType::NODE_INFO_RECEIVED, "upNoI")) {
        std::clog << "Debug: upNoI assignment!" << std::endl;

    }

    if (logicManager.assignAction<NodeEvent>(NodeEvent::IdType::NETWORK_INFO_RECEIVED, "upNeI")) {
        std::clog << "Debug: upNoI assignment!" << std::endl;

    }

    if (logicManager.assignAction<NodeEvent>(NodeEvent::IdType::NODE_INFO_RECEIVED, "addKnownNode")) {
        std::clog << "Debug: addKnownNode assignment!" << std::endl;

    }

    if (logicManager.assignAction<NodeEvent>(NodeEvent::IdType::NEW_NODE_DISCOVERED, "nodeDiscovered")) {
        std::clog << "Debug: addKnownNode assignment!" << std::endl;

    }

    if (logicManager.assignAction<Tick>(1500ms, "trigNodeUp")) {
        std::clog << "Debug: TtrigNodeUp assignment!" << std::endl;

    }


//
//    IProtocol* protocol = new DummyProtocol();
//    protocol->setupLogic(logicManager);

}


int main() {

    int ii = 7;
    RefX i(ii);
    i.getA() = 5;
    LOGGER(std::to_string(i.getA()));


    LogicManager logicManager;
    logicManager.addSource<ClockSource>();
    logicManager.addSource<NetworkSource>();

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
    thirdNode.start();

    thisNode.connectTo("127.0.0.1:9999");
    otherNode.connectTo("127.0.0.1:9898");


    std::this_thread::sleep_for(300s);
    return 0;
}
