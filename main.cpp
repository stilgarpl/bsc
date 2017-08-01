#include <iostream>
#include <thread>
#include "network/node/Node.h"


using namespace std::chrono_literals;

#include <fstream>

class SerialTest {

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


    std::shared_ptr<NetworkPacket> np1 = std::make_shared<NetworkPacketExtreme>();
    std::shared_ptr<NetworkPacket> np2 = std::make_shared<NetworkPacket>();
    std::shared_ptr<NetworkPacketExtreme> np3 = std::make_shared<NetworkPacketExtreme>();

    np1->dd = 7;
    np2->dd = 8;
    np3->a = 99;
    Node node, node1;
    node.listen();
    Poco::Net::SocketAddress address("127.0.0.1:6777");
    ClientConnection connection = node1.connectTo(address);

    connection.send(np1);


    connection.send(np2);
    connection.send(np3);
    std::this_thread::sleep_for(2s);
    connection.send(np1);
    std::this_thread::sleep_for(30s);
    return 0;
}