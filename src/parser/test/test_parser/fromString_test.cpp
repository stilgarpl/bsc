//
// Created by Krzysztof Tulidowicz on 09.01.2020.
//

#include <catch2/catch.hpp>
#include <list>
#include <map>
#include <parser/parser/fromString.h>

using namespace bsc;

enum class TestEnum {
    one,
    two,
    three,
};

struct CustomClassType {
    int a, b, c;
    std::string x;
};

namespace bsc {

    template<>
    class ParserImplementation<CustomClassType> {
    public:
        ParserImplementation(const Parser&){};

        [[nodiscard]] CustomClassType parse(const StringType& value) {
            Parser parser;
            auto list = parser.fromString<std::vector<int>>(value);
            if (list.size() != 3) {
                throw StringParseException("Failed to parse CustomClassType");
            }
            return CustomClassType{list[0], list[1], list[2], "parsed"};
        };
    };
}// namespace bsc

TEST_CASE("FromString parser test") {
    SECTION("Default parser configuration") {
        Parser parser;
        int five         = parser.fromString<int>("5");
        float sixAndHalf = parser.fromString<float>("6.5");
        std::string text = parser.fromString<std::string>("text");
        auto pair        = parser.fromString<std::pair<std::string, int>>("text=99");
        auto vector      = parser.fromString<std::vector<int>>("1,2,37");
        auto map         = parser.fromString<std::map<int, int>>("2=5,9=13");
        auto testEnum    = parser.fromString<TestEnum>("three");
        auto testDate1   = parser.fromString<std::chrono::sys_days>("2022-03-29");
        auto testDate2   = parser.fromString<std::chrono::sys_seconds>("2019-05-11 8:11:32");
        auto ymd1        = std::chrono::year_month_day{testDate1};
        auto ymd2        = std::chrono::year_month_day(std::chrono::floor<std::chrono::days>(testDate2));

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
        REQUIRE(static_cast<int>(ymd1.year()) == 2022);
        REQUIRE(static_cast<unsigned>(ymd1.month()) == 3);
        REQUIRE(static_cast<unsigned>(ymd1.day()) == 29);
        REQUIRE(static_cast<int>(ymd2.year()) == 2019);
        REQUIRE(static_cast<unsigned>(ymd2.month()) == 5);
        REQUIRE(static_cast<unsigned>(ymd2.day()) == 11);
        REQUIRE_THROWS_MATCHES(parser.fromString<int>("qqq"),
                               StringParseException,
                               Catch::Matchers::Message("Integer parsing failed for value: qqq"));
    }
    SECTION("Custom parser configuration") {
        Parser parser({.csvDelimiter = ';', .pairDelimiter = '@'});
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

    SECTION("Custom parser implementation") {
        Parser parser;
        auto result = parser.fromString<CustomClassType>("4,99,1");
        REQUIRE(result.a == 4);
        REQUIRE(result.b == 99);
        REQUIRE(result.c == 1);
        REQUIRE(result.x == "parsed");
    }
}