#include <iostream>
#include <thread>

//#define CEREAL_THREAD_SAFE 1
#include "network/node/Node.h"
#include "logic/sources/ClockSource.h"
#include "network/logic/sources/NetworkSource.h"
#include "network/protocol/logic/sources/AuthSource.h"
#include "network/protocol/logic/sources/ConnectionSource.h"
#include "network/protocol/logic/actions/TransmissionControl.h"
#include "network/logic/actions/ProtocolActions.h"


using namespace std::chrono_literals;

#include <fstream>


void setupProtocolLogic(LogicManager &logicManager, TransmissionControl &transmissionControl) {
    //adding sources
    logicManager.addSource<AuthSource>();
    logicManager.addSource<ClockSource>();
    logicManager.addSource<ConnectionSource>();



    //setting actions
    logicManager.setAction<ConnectionEvent>("connDebug", [](Context &, const ConnectionEvent &event) {

        std::clog << "Debug: connection event!" << std::endl;
    });

    logicManager.setAction<PacketEvent>(PacketEventId::PACKET_RECEIVED,
                                        [&transmissionControl](Context &ctx,
                                                               const PacketEvent &packetEvent) {
                                            return transmissionControl.onPacketReceived(ctx, packetEvent);
                                        });
    logicManager.setAction<PacketEvent>(PacketEventId::PACKET_SENT, [&transmissionControl](Context &ctx,
                                                                                           const PacketEvent &packetEvent) {
        return transmissionControl.onPacketSent(ctx, packetEvent);
    });

    logicManager.setAction<Tick>("TransTick", [&transmissionControl](Context &ctx, const Tick &tick) {
        return transmissionControl.work(ctx, tick);
    });


    logicManager.setAction<ConnectionEvent>("onConnect", ProtocolActions::onNewConnection);

    //assigning actions
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


    if (logicManager.assignAction<ConnectionEvent>("onConnect")) {
        std::clog << "Debug: onConn assignment!" << std::endl;

    }


}


int main() {

    LogicManager logicManager;
    logicManager.addSource<ClockSource>();
    logicManager.addSource<NetworkSource>();

    logicManager.setAction<Tick>("test", [](Context &, const Tick &t) {

        std::clog << "test tick " << t.getEventId().count() << std::endl;
    });


    Context context;
    TransmissionControl transmissionControl;
    context.setKey<std::string>(
            "test", "wartosc");
    std::clog << "Test context : " << *(context.get<std::string>("test")) << std::endl;
    context.set<int>(0);
    Node thisNode(9191);
    setupProtocolLogic(thisNode.getLogicManager(), transmissionControl);
    thisNode.start();

    Node otherNode(9999);
    setupProtocolLogic(otherNode.getLogicManager(), transmissionControl);
    otherNode.start();

    thisNode.connectTo("127.0.0.1:9999");


    std::this_thread::sleep_for(30s);
    return 0;
}
