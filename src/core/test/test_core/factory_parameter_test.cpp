//
// Created by Krzysztof Tulidowicz on 09.12.2021.
//

#include "core/factory/FactoryParameter.h"
#include "parser/parameters/CommandLineParameters.h"
#include <catch2/catch.hpp>
#include <core/factory/CopyFactory.h>
#include <core/factory/FactoryPtr.h>
#include <core/factory/ParsingFactory.h>
#include <list>
#include <sstream>
#include <core/optional/optional.h>

using namespace bsc;

namespace bsc {
    template<>
    struct FactoryTraits<int, NoFactorySpecialization> {
        using SelectorType = std::string;
        using ArgumentType = std::vector<std::string>;
    };

    using IntFactory = ParsingFactory<int>;
}// namespace bsc

template<typename T>
FactoryPtr<T> getFactory() {
    FactoryPtr<T> factory;
    return factory;
}

TEST_CASE("Factory Parameter test") {
    auto context = Context::makeContext();
    Context::setActiveContext(context);
    FactoryPtr<int> intFactory;
    intFactory.as<IntFactory>()->registerCreator("x77", 77);
    intFactory.as<IntFactory>()->registerCreator("x80", 80);
    intFactory.as<IntFactory>()->registerCreator("xx", *[](const int& a){
        return a;
    });

    struct TestParameters : CommandLineParameters {
        DefaultParameter<std::string> argument = {{.shortKey = 'a', .longKey = "argument", .argumentName = "INT", .doc = "argument for factory int", .defaultValue="0"}};
        FactoryParameter<int> factoryInt = {
                {.shortKey = 'f', .longKey = "factoryInt", .argumentName = "INT", .doc = "FACTORY", .factory = getFactory<int>(), .factoryDetails = {.argumentFetcher = [this]() {return std::vector{this->argument()};}}}};
        FactoryParameter<std::vector<int>> factoryListInt = {
                {.shortKey = 'F', .longKey = "factoryListInt", .argumentName = "INT", .doc = "FACTORY"}};
    };

    CommandLineParser silentParser{ParseConfiguration::silent, {}};
    SECTION("invalid factory selector") {
        std::vector<std::string> arg = {"-f", "78"};
        REQUIRE_THROWS_MATCHES(silentParser.parse<TestParameters>("cmd", arg),
                               ValueNotAllowed,
                               Catch::Matchers::Message("Value 78 is not allowed."));
    }

    SECTION("invalid factory list selector") {
        std::vector<std::string> arg = {"-F", "78"};
        REQUIRE_THROWS_MATCHES(silentParser.parse<TestParameters>("cmd", arg),
                               ValueNotAllowed,
                               Catch::Matchers::Message("Value 78 is not allowed."));
    }
    SECTION("valid factory selector") {
        std::vector<std::string> arg = {"-f", "x77"};
        const auto& params           = silentParser.parse<TestParameters>("cmd", arg);
        REQUIRE(params.factoryInt() == 77);
        REQUIRE(params.factoryInt.getSelector() == "x77");
    }

    SECTION("valid factory selector with argument") {
        std::vector<std::string> arg = {"-f", "xx", "-a", "68"};
        const auto& params           = silentParser.parse<TestParameters>("cmd", arg);
        REQUIRE(params.factoryInt() == 68);
        REQUIRE(params.factoryInt.getSelector() == "xx");
    }

    SECTION("valid factory list selector") {
        std::vector<std::string> arg = {"-F", "x77", "-F", "x80,x77"};
        const auto& params           = silentParser.parse<TestParameters>("cmd", arg);
        REQUIRE(params.factoryListInt().has_value());
        const auto list = params.factoryListInt().value();
        const auto selectors = params.factoryListInt.getSelector() | optional::orElseThrow(std::invalid_argument("bad selectors"));
        const auto selectorVector = selectors | ranges::to<std::vector>();

        REQUIRE(list.size() == 3);
        REQUIRE(selectorVector.size() == 3);
        REQUIRE(list[0] == 77);
        REQUIRE(selectorVector[0] == "x77");
        REQUIRE(list[1] == 80);
        REQUIRE(selectorVector[1] == "x80");
        REQUIRE(list[2] == 77);
        REQUIRE(selectorVector[2] == "x77");
    }
}