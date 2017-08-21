#include <iostream>
#include <thread>

//#define CEREAL_THREAD_SAFE 1
#include "network/node/Node.h"
#include "network/protocol/packet/CommandPacket.h"


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
    std::cout << "Hello, World!" << std::endl;

    std::stringstream str;

    char x1[50] = "lalalal test";
    char x2[50];

//    NetworkServiceManager nsm2;

    std::shared_ptr<std::string> temp1 = nsm(temp1);



//
//    *temp1 = "dupa";

    //std::cout << *temp2 << std::endl;

    std::queue<std::shared_ptr<std::string>> q;
    q.push(std::make_shared<std::string>("dupa1"));
    q.push(std::make_shared<std::string>("dupa2"));
    q.push(std::make_shared<std::string>("dupa3"));

    auto p = q.front();
    q.pop();

    std::cout << *p << std::endl;


    std::queue<std::shared_ptr<Des>> q1;
    q1.push(std::make_shared<Des>());
    q1.push(std::make_shared<Des>());
    q1.push(std::make_shared<Des>());
    auto p1 = q1.front();
    // auto p2 = p1;
    std::cout << "F" << std::endl;
    q1.pop();
    std::cout << "P" << std::endl;


    //  str << x1;

//    SerialTest st(1,2,3,"lal");
//    {
//        cereal::JSONOutputArchive oarchive(str);
//        oarchive(st);
//        std::cout << str.str();
//    }
//
//
//
//    SerialTest t2(2,2,2,"dupa");
//    ApplicationProtocol::send(t2);
//
//    {
//        cereal::JSONInputArchive iarchive(str);
//        iarchive(t2);
//    }
//
//    //oa << st;
//
//
//    ApplicationProtocol::send(t2);

    Context context;
    context.set<int, int>(0, 6);// = std::make_shared<int>(5);

    auto ddd = context.get<int, int>(0, 6);// = std::make_shared<int>(5);


    context.set<int>(0, 9);
    *ddd = 1;
    Context context2;
    //  auto ddd2 = context2.get<int>(0);// = std::make_shared<int>(5);
    //  *ddd2 = 8;

    std::cout << "CONTEXT TEST " << *context.get<int>(0) << " " << *ddd << std::endl;
    //  std::cout << "CONTEXT TEST " << *context2.get<int>(0) << std::endl;
    //   exit(0);
    std::shared_ptr<CommandPacket> np2 = std::make_shared<CommandPacket>();
//    std::shared_ptr<NetworkPacketExtreme> np3 = std::make_shared<NetworkPacketExtreme>();
//
//    np1->dd = 7;
//    np2->dd = 8;
//    np3->a = 99;
    std::shared_ptr<CommandPacket> np1 = std::make_shared<CommandPacket>();
    Node node, node1;
    node.listen();
    Poco::Net::SocketAddress address("127.0.0.1:6777");
    Connection connection;
    std::this_thread::sleep_for(300ms);
    Poco::Net::StreamSocket socket1(address);
    ConnectionProcessor cp(connection);
    cp.start();
    connection.startReceiving(socket1);
    connection.startSending(socket1);
//    connection.send(np1);
//    connection.send(np1);
//    connection.send(np1);
//    connection.send(np1);
    //ClientConnection connection = node1.connectTo(address);
//    np1->setCommand(Command::ECHO);
//    connection.send(np1);
    np1->setCommand(Command::DISCOVERY);
    connection.send(np1);
    std::this_thread::sleep_for(500ms);
    np2->setCommand(Command::ECHO);
    connection.send(np2);
    connection.send(std::make_shared<NetworkPacket>());
    connection.send(std::make_shared<NetworkPacket>());
    connection.send(std::make_shared<NetworkPacket>());
    connection.send(std::make_shared<NetworkPacket>());
    connection.send(std::make_shared<NetworkPacket>());
    connection.send(std::make_shared<NetworkPacket>());
    connection.send(std::make_shared<NetworkPacket>());
    connection.send(std::make_shared<NetworkPacket>());
    connection.send(std::make_shared<NetworkPacket>());
    connection.send(std::make_shared<NetworkPacket>());

    // np1->setCommand(Command::GET_NODEINFO);
    //  connection.send(np1);
////
//
//    connection.send(np2);
//    connection.send(np3);
//    std::this_thread::sleep_for(2s);
//    connection.send(np1);
    std::this_thread::sleep_for(30s);
    return 0;
}