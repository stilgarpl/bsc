#include <iostream>
#include <thread>

//#define CEREAL_THREAD_SAFE 1
#include "network/node/Node.h"
#include "network/protocol/packet/CommandPacket.h"
#include "logic/sources/ClockSource.h"
#include "network/logic/sources/NetworkSource.h"
#include "network/protocol/logic/sources/AuthSource.h"
#include "network/protocol/logic/sources/ConnectionSource.h"
#include "network/logic/actions/ProtocolActions.h"
#include "network/protocol/logic/actions/TransmissionControl.h"


using namespace std::chrono_literals;

#include <fstream>


class Des {
public:
    virtual ~Des() {
        std::cout << "Desti" << std::endl;
    }
};

class A {

public:
    A() {
        std::cout << "A::" << std::endl;
    };

    A(int a) {
        std::cout << "A::int " << a << std::endl;
    };
};

class SerialTest {

public:
    std::shared_ptr<std::string> temp = nsm(temp);

private:
    int a, b, c;
    std::string dupa;
public:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(a), CEREAL_NVP(b), cereal::make_nvp("llala", c), dupa);
    }


public:
    SerialTest(int a, int b, int c, const std::string &dupa) : a(a), b(b), c(c), dupa(dupa) {}
};


int main() {

//    //EventSource eventSource;
//    Uber<std::vector> i;
//    i.get<int>().reserve(9);
//   // i.get<float>().resize(19);
//    i.get<std::string>().reserve(9);
//    i.get<int>()[5] = 7;
//    std::clog << "Uber test" << i.get<int>()[5] << std::endl;
//    i.get<float>().push_back(1.1f);
//    i.get<float>().push_back(1.2f);
//    i.get<float>().push_back(5.1f);
//
//    for (auto &&item : i.get<float>()) {
//        std::clog << "Uber test" << item << std::endl;
//    }
//
//
//    i.get<std::string>().push_back("dupa");
//    std::clog << "Uber test" << i.get<std::string>()[0] << std::endl;
//
//    Uber<std::list> l;
//    l.get<int>().push_back(5);
//    Uber<std::map> m;
//    m.get<int,int>()[5] = 7;
//    ISource source;
//    TempEvent event;
//    IEvent<int> ie;
//    ie.setEventId(4);
//    IEvent<float> fe;
//    fe.setEventId(3);
//    IEvent<std::string> se;
//    se.setEventId("dupa");
//    source.getSignal<TempEvent>(event.getEventId()).assign([](Context&,const TempEvent&){std::clog << "signal has signalled " << std::endl;});
//    source.getSignal<IEvent<int>>(ie.getEventId()).assign([](Context&,const IEvent<int>&){std::clog << "signal IE has signalled " << std::endl;});
//    source.getSignal<IEvent<float>>(fe.getEventId()).assign([](Context&,const IEvent<float>&){std::clog << "signal FE has signalled " << std::endl;});
//    source.getSignal<IEvent<float>>(fe.getEventId()).assign([](Context&,const IEvent<float>&){std::clog << "signal FE has signalled twice! " << std::endl;});
//    source.getSignal<IEvent<std::string>>(se.getEventId()).assign([](Context&,const IEvent<std::string>& e){std::clog << "signal SE has signalled! " + e.getEventId()  << std::endl;});
//   // source.assignSignal(ie.getEventId(),[](Context&,const IEvent<int>&){std::clog << "signal IE has signalled " << std::endl;});
//    source.event(event);
//    source.event(event);
//    source.event(event);
//    source.event(ie);
//    source.event(fe);
//    source.event(se);


    LogicManager logicManager;
    logicManager.addSource<ClockSource>();
    logicManager.addSource<NetworkSource>();

    logicManager.setAction<Tick>("test", [](Context &, const Tick &t) {

        std::clog << "test tick " << t.getEventId().count() << std::endl;
    });



//    am.setAction<Tick,int,float>("test",[](Context&,const Tick&,int,float){});
    //logicManager.getSourceManager().registerProvider<Tick>(&clock);
    //  logicManager.getActionManager().setAction<Tick>("dupa",[](Context &, const Tick &) { std::clog << "dupa " << std::endl; });
//    if (logicManager.assignAction<Tick>(1000ms, "test")) {
//        std::clog << "Assignment ok!" << std::endl;
//    }
//
//    if (logicManager.assignAction<Tick>(599ms, "test")) {
//        std::clog << "Assignment ok!" << std::endl;
//    }
//
//    if (logicManager.assignAction<Tick>(50ms, "test")) {
//        std::clog << "Assignment ok!" << std::endl;
//    }

    // manager.registerTrigger<Tick>([](Context &, const Tick &) { std::clog << "registered tick " << std::endl; });
    // manager.registerTrigger<Tick>(500ms,[](Context &, const Tick &) { std::clog << "tick every 500ms" << std::endl; });



//    clock.getSignal<Tick>(1000ms).assign(
//            [](Context &, const Tick &) { std::clog << "signal Tick has signalled 100ms " << std::endl; });
//    clock.getSignal<Tick>(3000ms).assign(
//            [](Context &, const Tick &) { std::clog << "signal Tick has signalled 300ms " << std::endl; });

//    for (int i =0 ; i< 10000; i++)
//        logicManager.work();

    Context context;
    TransmissionControl transmissionControl;

    context.setKey<std::string>(
            "test", "wartosc");
    std::clog << "Test context : " << *(context.get<std::string>("test")) << std::endl;
    context.set<int>(0);
    Node thisNode(9191);
    thisNode.getLogicManager().addSource<AuthSource>();
    thisNode.getLogicManager().addSource<ClockSource>();
    thisNode.getLogicManager().addSource<ConnectionSource>();
    thisNode.getLogicManager().setAction<ConnectionEvent>("connDebug", [](Context &, const ConnectionEvent &event) {

        std::clog << "Debug: connection event!" << std::endl;
    });

    thisNode.getLogicManager().setAction<PacketEvent>(PacketEventId::PACKET_RECEIVED,
                                                      [&transmissionControl](Context &ctx,
                                                                             const PacketEvent &packetEvent) {
                                                          return transmissionControl.onPacketReceived(ctx, packetEvent);
                                                      });
    thisNode.getLogicManager().setAction<PacketEvent>(PacketEventId::PACKET_SENT, [&transmissionControl](Context &ctx,
                                                                                                         const PacketEvent &packetEvent) {
        return transmissionControl.onPacketSent(ctx, packetEvent);
    });
    thisNode.getLogicManager().setAction<Tick>("TransTick", [&transmissionControl](Context &ctx, const Tick &tick) {
        return transmissionControl.work(ctx, tick);
    });
    if (thisNode.getLogicManager().assignAction<PacketEvent>(PacketEventId::PACKET_RECEIVED,
                                                             PacketEventId::PACKET_RECEIVED)) {
        std::clog << "Debug: PACK RECV assignment!" << std::endl;
    }
    thisNode.getLogicManager().assignAction<PacketEvent>(PacketEventId::PACKET_SENT, PacketEventId::PACKET_SENT);

    if (thisNode.getLogicManager().assignAction<Tick>(500ms, "TransTick")) {
        std::clog << "Debug: Trans tick assignment!" << std::endl;

    }

    if (thisNode.getLogicManager().assignAction<ConnectionEvent>("connDebug")) {
        std::clog << "Debug: ConEv assignment!" << std::endl;

    }
    thisNode.start();

    Node otherNode(9999);
    otherNode.getLogicManager().addSource<AuthSource>();
    otherNode.getLogicManager().addSource<ClockSource>();
    otherNode.getLogicManager().addSource<ConnectionSource>();
    otherNode.getLogicManager().setAction<ConnectionEvent>("connDebug", [](Context &, const ConnectionEvent &event) {

        std::clog << "Debug: connection event!" << std::endl;
    });

    otherNode.getLogicManager().setAction<PacketEvent>(PacketEventId::PACKET_RECEIVED,
                                                       [&transmissionControl](Context &ctx,
                                                                              const PacketEvent &packetEvent) {
                                                           return transmissionControl.onPacketReceived(ctx,
                                                                                                       packetEvent);
                                                       });
    otherNode.getLogicManager().setAction<PacketEvent>(PacketEventId::PACKET_SENT, [&transmissionControl](Context &ctx,
                                                                                                          const PacketEvent &packetEvent) {
        return transmissionControl.onPacketSent(ctx, packetEvent);
    });
    otherNode.getLogicManager().setAction<Tick>("TransTick", [&transmissionControl](Context &ctx, const Tick &tick) {
        return transmissionControl.work(ctx, tick);
    });
    otherNode.getLogicManager().assignAction<PacketEvent>(PacketEventId::PACKET_RECEIVED,
                                                          PacketEventId::PACKET_RECEIVED);
    otherNode.getLogicManager().assignAction<PacketEvent>(PacketEventId::PACKET_SENT, PacketEventId::PACKET_SENT);


    otherNode.getLogicManager().setAction<ConnectionEvent>("onConnect", ProtocolActions::onNewConnection);
    thisNode.getLogicManager().setAction<ConnectionEvent>("onConnect", ProtocolActions::onNewConnection);

    if (otherNode.getLogicManager().assignAction<ConnectionEvent>("connDebug")) {
        std::clog << "Debug: ConEv assignment!" << std::endl;

    }

    if (otherNode.getLogicManager().assignAction<Tick>(500ms, "TransTick")) {
        std::clog << "Debug: Trans tick assignment!" << std::endl;

    }
    if (otherNode.getLogicManager().assignAction<ConnectionEvent>("onConnect")) {
        std::clog << "Debug: onConn assignment!" << std::endl;

    }
    if (thisNode.getLogicManager().assignAction<ConnectionEvent>("onConnect")) {
        std::clog << "Debug: onConn assignment!" << std::endl;

    }
    otherNode.start();

    thisNode.connectTo("127.0.0.1:9999");


    std::this_thread::sleep_for(30s);
    return 0;
}
