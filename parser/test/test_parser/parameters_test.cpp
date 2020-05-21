//
// Created by stilgar on 15.01.2020.
//

#include <catch2/catch.hpp>
#include <parser/parameters/CommandLineParameters.h>

using namespace bsc;

struct TestParameters : CommandLineParameters {

    Flag flag                                 = {{'f', "flag", "Flag"}};
    DefaultParameter<std::string> text        = {{'t', "text", "text", "Text", "Default"}};
    DefaultParameter<std::string> shortText   = {{'s', "short", "short text", "default"}};
    DefaultParameter<std::string> defaultText = {{'d', "short", "short text", "default"}};
    Parameter<std::vector<int>> vector        = {{'v', "vec", "vec", "Vector of ints"}};
    Argument<int> number;
};

TEST_CASE("Parameters test") {
    using namespace std::string_literals;
    std::vector<std::string> arg = {"-f"s, "-s", "short", "-t"s, "lala"s, "-v"s, "1,2,7"s, "5"s};
    const auto& params = CommandLineParameters::parse<TestParameters>("cmd"s, arg, ParseConfiguration::silent);
    REQUIRE(params.flag() == true);
    REQUIRE(params.text() == "lala");
    REQUIRE(params.vector().has_value());
    REQUIRE(params.vector()->size() == 3);
    REQUIRE(params.vector()->at(0) == 1);
    REQUIRE(params.vector()->at(1) == 2);
    REQUIRE(params.vector()->at(2) == 7);
    REQUIRE(params.number() == 5);
    REQUIRE(params.shortText() == "short");
    REQUIRE(params.defaultText() == "default");
}