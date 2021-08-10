//
// Created by Krzysztof Tulidowicz on 21.01.2021.
//
#include <catch2/catch.hpp>
#include <context/context/Context.h>
#include <properties/Property.h>
#include <properties/PropertyContext.h>
#include <properties/control/writer/YamlWriter.h>
#include <properties/control/parser//YamlParser.h>

using namespace bsc;

TEST_CASE("Yaml writer test") {
    YamlWriter writer;

    writer.selectNode({"a", "b"});
    writer.setValue("value");
    auto result = writer.writeToString();

    REQUIRE(!result.empty());
    REQUIRE(result == "a:\n  b: value");

}
