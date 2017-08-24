#include <iostream>
#include <thread>

//#define CEREAL_THREAD_SAFE 1
#include "network/node/Node.h"
#include "network/protocol/packet/CommandPacket.h"
#include "logic/ISource.h"
#include "logic/sources/ClockSource.h"
#include "logic/SourceManager.h"


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

    SourceManager manager;
    ClockSource clock;
    manager.registerProvider<Tick>(&clock);

    manager.registerTrigger<Tick>([](Context &, const Tick &) { std::clog << "registered tick " << std::endl; });
    manager.registerTrigger<Tick>(500ms,
                                  [](Context &, const Tick &) { std::clog << "registered tick 2" << std::endl; });



    clock.getSignal<Tick>(1000ms).assign(
            [](Context &, const Tick &) { std::clog << "signal Tick has signalled 100ms " << std::endl; });
    clock.getSignal<Tick>(3000ms).assign(
            [](Context &, const Tick &) { std::clog << "signal Tick has signalled 300ms " << std::endl; });
    
    for (int i =0 ; i< 10000; i++)
    clock.work();

    Context context;

    context.setKey<std::string>(
            "test", "wartosc");
    std::clog << "Test context : " << *(context.get<std::string>("test")) << std::endl;
    context.set<int>(0);
    Node thisNode;
    thisNode.start();
    std::this_thread::sleep_for(30s);
    return 0;
}
