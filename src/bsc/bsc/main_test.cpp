
#include <iostream>
#include <yaml-cpp/yaml.h>
using namespace std::string_literals;

template<typename X>
class A {};

int main(int argc, char* argv[]) {

    YAML::Node node = YAML::Load("a:\n  b: test");
    std::cout << node["a"]["b"] << std::endl;
    std::cout << node["a.b"] << std::endl;
    return 0;
}
