#include <filesystem>
#include <iostream>
#include <list>
#include <parser/parameters/CommandLineParameters.h>
#include <set>
#include <thread>

using namespace std::chrono_literals;
using namespace bsc;
using namespace std::string_literals;

struct TestParameters : CommandLineParameters {

    Flag flag                                 = {{'f', "flag", "Flag"}};
    DefaultParameter<std::string> text        = {{'t', "text", "text", "Text", "Default"}};
    DefaultParameter<std::string> shortText   = {{'s', "short", "short text", "default"}};
    DefaultParameter<std::string> defaultText = {{'d', "short", "short text", "default"}};
    Parameter<std::vector<int>> vector        = {{'v', "vec", "vec", "Vector of ints"}};
    Argument<int> number;
};

struct TestProgramParameters : public bsc::CommandLineParameters {
    Parameter<int> a = {{
            'a',
            "test",
            "NUM",
            "Int value",
    }};
    Group g1         = {"Group 1"};
    Flag b           = {{'b', "bool", "Bool value"}};

    //    Parameter<int> c = {"test"};
    Parameter<std::string> d                    = {{.shortKey      = 'd',
                                 .longKey       = "test",
                                 .argumentName  = "TEXT",
                                 .doc           = "Text field",
                                 .defaultValue  = "xix",
                                 .allowedValues = []() {
                                     return std::set<std::string>{"eee"s, "xxx"s};
                                 }}};
    Parameter<std::string> dd                   = {{.shortKey      = 'D',
                                  .longKey       = "testd",
                                  .argumentName  = "TEXTd",
                                  .doc           = "Text fieldd",
                                  .defaultValue  = "xixd",
                                  .allowedValues = {"lala", "lulu"}}};
    Parameter<std::vector<std::string>> vec     = {{'v', "vector", "VEC", "Vector test"}};
    Parameter<std::vector<int>> vin             = {{'q', "qqqwer", "VEC", "Vector int test"}};
    Alias a1                                    = {'x', "aaaaaa"};
    Parameter<std::list<std::string>> list      = {{'l', "list", "LIS", "List test"}};
    Parameter<std::set<std::string>> set        = {{'s', "set", "SET", "ssss"}};
    Parameter<std::map<std::string, int>> map   = {{'m', "map", "MAP", "maaap"}};
    Parameter<std::pair<std::string, int>> pair = {{'p', "pair", "PAIR", "Paaaior"}};
    Parameter<std::filesystem::path> path       = {{'o', "path", "PATH", "paath"}};
};

#include <core/utils/cereal_include.h>

int main(int argc, char* argv[]) {

    const auto& params = CommandLineParser::defaultParse<TestParameters>(argc, argv);
    std::stringstream dataStorage;
    cereal::BinaryOutputArchive oa(dataStorage);
    cereal::BinaryInputArchive ia(dataStorage);

    // given
    std::filesystem::path path = "/home/";

    oa << path;

    std::filesystem::path newPath;
    ia >> newPath;

    return 0;
}
