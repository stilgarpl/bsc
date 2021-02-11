//
// Created by Krzysztof Tulidowicz on 15.01.2020.
//

#include <catch2/catch.hpp>
#include <list>
#include <parser/parameters/CommandLineParameters.h>

using namespace bsc;

struct TestParameters : CommandLineParameters {

    Flag flag                          = {{.shortKey = 'f', .longKey = "flag", .doc = "Flag"}};
    DefaultParameter<std::string> text = {
            {.shortKey = 't', .longKey = "text", .argumentName = "text", .doc = "Text", .defaultValue = "Default"}};
    DefaultParameter<std::string> shortText = {
            {.shortKey = 's', .argumentName = "short", .doc = "short text", .defaultValue = "default", .allowedValues = {"short"}}};
    DefaultParameter<std::string> defaultText = {
            {.shortKey = 'd', .argumentName = "short", .doc = "short text", .defaultValue = "default"}};
    Parameter<std::vector<int>> vector = {{'v', "vec", "vec", "Vector of ints"}};
    Argument<int> number;
    Parameter<std::map<int, int>> map = {{.shortKey = 'm', .longKey = "map", .argumentName = "map"}};
    Arguments<std::list<std::string>> other;
};

TEST_CASE("Parameters test") {
    using namespace std::string_literals;
    std::vector<std::string> arg = {"-f", "-s", "short", "-t", "la la", "-v", "1,2,7", "5"s, "argument"};
    CommandLineParser silentParser{ParseConfiguration::silent, {}};
    const auto& params = silentParser.parse<TestParameters>("cmd", arg);
    REQUIRE(params.commandName() == "cmd");
    REQUIRE(params.flag() == true);
    REQUIRE(params.text() == "la la");
    REQUIRE(params.vector().has_value());
    REQUIRE(params.vector()->size() == 3);
    REQUIRE(params.vector()->at(0) == 1);
    REQUIRE(params.vector()->at(1) == 2);
    REQUIRE(params.vector()->at(2) == 7);
    REQUIRE(params.number() == 5);
    REQUIRE(!params.map().has_value());
    REQUIRE(params.shortText() == "short");
    REQUIRE(params.defaultText() == "default");
    std::vector<std::string> expectedArguments          = {{"5"}, {"argument"}};
    std::vector<std::string> expectedRemainingArguments = {{"argument"}};
    REQUIRE(params.arguments() == expectedArguments);
    REQUIRE(params.remainingArguments()[0] == std::span(expectedRemainingArguments)[0]);
    REQUIRE(params.remainingArguments().size() == std::span(expectedRemainingArguments).size());
    REQUIRE(params.other().size() == 1);
    REQUIRE(*(params.other().begin()) == "argument");
}

TEST_CASE("Parameters test with custom parser") {
    Parser parser({.csvDelimiter = ';', .pairDelimiter = '@'});
    using namespace std::string_literals;
    std::vector<std::string> arg = {"-f"s, "-s"s, "short"s, "-t"s, "la la"s, "-v"s, "1;2;7"s, "-m", "2@3;5@1;9@0", "5"s, "argument"};
    CommandLineParser silentParser{ParseConfiguration::silent, parser};
    const auto& params = silentParser.parse<TestParameters>("cmd"s, arg);
    REQUIRE(params.flag() == true);
    REQUIRE(params.text() == "la la");
    REQUIRE(params.vector().has_value());
    REQUIRE(params.vector()->size() == 3);
    REQUIRE(params.vector()->at(0) == 1);
    REQUIRE(params.vector()->at(1) == 2);
    REQUIRE(params.vector()->at(2) == 7);
    REQUIRE(params.number() == 5);
    REQUIRE(params.map()->size() == 3);
    REQUIRE(params.map()->at(2) == 3);
    REQUIRE(params.map()->at(5) == 1);
    REQUIRE(params.map()->at(9) == 0);
    REQUIRE(params.shortText() == "short");
    REQUIRE(params.defaultText() == "default");
    std::vector<std::string> expectedArguments          = {{"5"}, {"argument"}};
    std::vector<std::string> expectedRemainingArguments = {{"argument"}};
    REQUIRE(params.arguments() == expectedArguments);
    REQUIRE(params.remainingArguments()[0] == std::span(expectedRemainingArguments)[0]);
    REQUIRE(params.remainingArguments().size() == std::span(expectedRemainingArguments).size());
    REQUIRE(params.other().size() == 1);
    REQUIRE(*(params.other().begin()) == "argument");
}

TEST_CASE("Invalid parameters test") {
    using namespace std::string_literals;
    CommandLineParser silentParser{ParseConfiguration::silent, {}};
    std::vector<std::string> arg = {"-f"s, "-s"s, "invalid value"s, "-t"s, "lala"s, "-v"s, "1,2,7"s, "5"s};
    REQUIRE_THROWS_AS(silentParser.parse<TestParameters>("cmd"s, arg), ValueNotAllowed);
}