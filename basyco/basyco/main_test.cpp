#include <iostream>
#include <thread>
#include <core/parameters/ProgramParameters.h>


using namespace std::chrono_literals;



struct TestProgramParameters : public ProgramParameters {
    Parameter<int> a = {'a', "test", "NUM", "Int value",};
    Group g1 = {"Group 1"};
    Flag b = {'b', "bool", "Bool value"};
    Alias a1 = {'x', "aaaaaa"};
//    Parameter<int> c = {"test"};
    Parameter<std::string> d = {'d', "test", "TEXT", "Text field", "xix"};
};

int main(int argc, char* argv[]) {

    TestProgramParameters parameters;
    parameters.usage("la xxx");
    parameters.usage("aaaaaa");
    parameters.before("qewwqewewq");
    parameters.after("ewrefesfezfse");
    parameters.parse(argc, argv);
    std::cout << "b is " << (parameters.b() ? "true" : "false ") << " with count "
              << std::to_string(parameters.b.count()) << std::endl;
    std::cout << "a is " << std::to_string(parameters.a()) << std::endl;
    std::cout << "d is [" << parameters.d() << "]" << std::endl;
    std::cout << "arguments are : " << std::endl;
    for (const auto& argument : parameters.arguments()) {
        std::cout << "[" << argument << "] ";
    }
    std::cout << std::endl;


    return 0;

}


