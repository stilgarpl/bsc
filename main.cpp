#include <iostream>
#include <thread>
#include "network/node/Node.h"
#include "network/protocol/ApplicationProtocol.h"

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

    Node node;
    node.listen();
    std::this_thread::sleep_for(30s);
    return 0;
}