//
// Created by stilgar on 09.01.2020.
//

#include <catch2/catch.hpp>
#include <map>
#include <parser/parser/fromString.h>

TEST_CASE("FromString parser test") {
    int five = fromString<int>("5");
    float sixAndHalf = fromString<float>("6.5");
    std::string text = fromString<std::string>("text");
    auto pair = fromString<std::pair<std::string, int>>("text=99");
    auto vector = fromString<std::vector<int>>("1,2,37");
    auto map = fromString<std::map<int, int>>("2=5,9=13");

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
    REQUIRE_THROWS_MATCHES(fromString<int>("qqq"), StringParseException,
                           Catch::Matchers::Message("Integer parsing failed for value: qqq"));

}