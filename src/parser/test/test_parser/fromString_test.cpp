//
// Created by Krzysztof Tulidowicz on 09.01.2020.
//

#include <catch2/catch.hpp>
#include <map>
#include <parser/parser/fromString.h>

using namespace bsc;

enum class TestEnum {
    one,
    two,
    three,
};

TEST_CASE("FromString parser test") {
    SECTION("Default parser configuration") {
        Parser parser;
        int five         = parser.fromString<int>("5");
        float sixAndHalf = parser.fromString<float>("6.5");
        std::string text = parser.fromString<std::string>("text");
        auto pair        = parser.fromString<std::pair<std::string, int>>("text=99");
        auto vector      = parser.fromString<std::vector<int>>("1,2,37");
        auto map         = parser.fromString<std::map<int, int>>("2=5,9=13");
        auto testEnum   = parser.fromString<TestEnum>("three");

        REQUIRE(five == 5);
        REQUIRE(sixAndHalf == 6.5f);
        REQUIRE(text == "text");
        REQUIRE(pair.first == "text");
        REQUIRE(pair.second == 99);
        REQUIRE(vector.size() == 3);
        REQUIRE(vector[0] == 1);
        REQUIRE(vector[1] == 2);
        REQUIRE(vector[2] == 37);
        REQUIRE(map.size() == 2);
        REQUIRE(map.at(2) == 5);
        REQUIRE(map.at(9) == 13);
        REQUIRE(testEnum == TestEnum::three);
        REQUIRE_THROWS_MATCHES(parser.fromString<int>("qqq"),
                               StringParseException,
                               Catch::Matchers::Message("Integer parsing failed for value: qqq"));
    }
    SECTION("Custom parser configuration") {
        Parser parser({.csvDelimiter = ';',.pairDelimiter='@'});
        int five         = parser.fromString<int>("5");
        float sixAndHalf = parser.fromString<float>("6.5");
        std::string text = parser.fromString<std::string>("text");
        auto pair        = parser.fromString<std::pair<std::string, int>>("text@98");
        auto vector      = parser.fromString<std::vector<int>>("1;2;37");
        auto map         = parser.fromString<std::map<int, int>>("2@5;9@14");

        REQUIRE(five == 5);
        REQUIRE(sixAndHalf == 6.5f);
        REQUIRE(text == "text");
        REQUIRE(pair.first == "text");
        REQUIRE(pair.second == 98);
        REQUIRE(vector.size() == 3);
        REQUIRE(vector[0] == 1);
        REQUIRE(vector[1] == 2);
        REQUIRE(vector[2] == 37);
        REQUIRE(map.size() == 2);
        REQUIRE(map.at(2) == 5);
        REQUIRE(map.at(9) == 14);
        REQUIRE_THROWS_MATCHES(parser.fromString<int>("qqq"),
                               StringParseException,
                               Catch::Matchers::Message("Integer parsing failed for value: qqq"));
    }

}