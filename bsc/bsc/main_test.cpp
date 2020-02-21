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

#include <core/utils/cereal_include.h>

int main(int argc, char* argv[]) {

    std::stringstream dataStorage;
    cereal::BinaryOutputArchive oa(dataStorage);
    cereal::BinaryInputArchive ia(dataStorage);

    //given
    std::filesystem::path path = "/home/";

    oa << path;

    std::filesystem::path newPath;
    ia >> newPath;


    return 0;

}


