#include <iostream>
#include <thread>
#include <parser/parameters/CommandLineParameters.h>
#include <list>
#include <set>
#include <filesystem>



using namespace std::chrono_literals;
using namespace bsc;



struct TestProgramParameters : public bsc::CommandLineParameters {
    Parameter<int> a = {'a', "test", "NUM", "Int value",};
    Group g1 = {"Group 1"};
    Flag b = {'b', "bool", "Bool value"};

//    Parameter<int> c = {"test"};
    Parameter<std::string> d = {'d', "test", "TEXT", "Text field", "xix"};
    Parameter<std::vector<std::string>> vec = {'v', "vector", "VEC", "Vector test"};
    Parameter<std::vector<int>> vin = {'q', "qqqwer", "VEC", "Vector int test"};
    Alias a1 = {'x', "aaaaaa"};
    Parameter<std::list<std::string>> list = {'l', "list", "LIS", "List test"};
    Parameter<std::set<std::string>> set = {'s', "set", "SET", "ssss"};
    Parameter<std::map<std::string, int>> map = {'m', "map", "MAP", "maaap"};
    Parameter<std::pair<std::string, int>> pair = {'p', "pair", "PAIR", "Paaaior"};
    Parameter<std::filesystem::path> path = {'o', "path", "PATH", "paath"};
};

int main(int argc, char* argv[]) {


//    parameters.usage("la xxx");
//    parameters.usage("aaaaaa");
//    parameters.before("qewwqewewq");
//    parameters.after("ewrefesfezfse");

    auto parameters = bsc::CommandLineParameters::parse<TestProgramParameters>(argc, argv,
                                                                               {{"lallala"}, std::nullopt, "qqww"});
    std::cout << "b is " << (parameters.b() ? "true" : "false ") << " with count "
              << std::to_string(parameters.b.count()) << std::endl;
    std::cout << "a is " << std::to_string(parameters.a().value_or(-1)) << std::endl;
    std::cout << "d is [" << parameters.d().value_or("_") << "]" << std::endl;
    std::cout << "path is [" << parameters.path().value_or("/").string() << "]" << std::endl;
    if (parameters.pair()) {
        std::cout << "Pair is " << parameters.pair()->first << "=[" << parameters.pair()->second << "]" << std::endl;
    }
    std::cout << "v elements are : " << std::endl;
    if (parameters.vec()) {
        for (const auto& v : *parameters.vec()) {
            std::cout << "[" << v << "] ";
        }
    }
    std::cout << std::endl;
    std::cout << "list elements are : " << std::endl;
    if (parameters.list()) {
        for (const auto& v : *parameters.list()) {
            std::cout << "[" << v << "] ";
        }
    }
    std::cout << std::endl;

    std::cout << "vin elements are : " << std::endl;
    if (parameters.vin()) {
        for (const auto& v : *parameters.vin()) {
            std::cout << "[" << std::to_string(v) << "] ";
        }
    }
    std::cout << std::endl;
    std::cout << "set elements are : " << std::endl;
    if (parameters.set()) {
        for (const auto& v : *parameters.set()) {
            std::cout << "[" << v << "] ";
        }
    }
    std::cout << std::endl;

    std::cout << "map elements are : " << std::endl;
    if (parameters.map()) {
        for (const auto& v : *parameters.map()) {
            std::cout << v.first << "=[" << std::to_string(v.second) << "] ";
        }
    }
    std::cout << std::endl;
    std::cout << "arguments are : " << std::endl;
    for (const auto& argument : parameters.arguments()) {
        std::cout << "[" << argument << "] ";
    }
    std::cout << std::endl;


    return 0;

}


