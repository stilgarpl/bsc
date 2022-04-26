//
// Created by Krzysztof Tulidowicz on 25.04.22.
//

#include "parser/value/DateTimeValue.h"
#include <catch2/catch.hpp>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
using namespace bsc;

TEST_CASE("DateTimeValue test") {
    using namespace std::chrono_literals;
    bsc::DateTimeValue d1{std::chrono::sys_seconds{130s}, DateTimeValue::Precision::seconds};
    bsc::DateTimeValue d2{std::chrono::sys_seconds{121s}, DateTimeValue::Precision::minutes};
    bsc::DateTimeValue d3{std::chrono::sys_seconds{121s}, DateTimeValue::Precision::seconds};

    REQUIRE(d1 == d2);
    REQUIRE(d1 > d3);
    REQUIRE(d2 == d3);
}

std::string getFromStream(const DateTimeValue& t) {
    std::stringstream s;
    s << t;
    return s.str();
}

TEST_CASE("DateTimeValue parser test") {
    auto currentYear = std::to_string(
            std::chrono::year_month_day(std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())).year().operator int());
    auto value1 = bsc::fromString<bsc::DateTimeValue>(currentYear);
    auto value2 = bsc::fromString<bsc::DateTimeValue>(currentYear + "-04-25");
    auto value3 = bsc::fromString<bsc::DateTimeValue>(currentYear + "-05-09");
    auto value4 = bsc::fromString<bsc::DateTimeValue>(currentYear + "-5-09"); //does not work with -5-9
    bsc::DateTimeValue d1{std::chrono::file_clock ::now()};
    REQUIRE(value1 == d1);
    REQUIRE(d1 >= value1);
    REQUIRE(value1 == value2);
    REQUIRE(value1 == value3);
    REQUIRE(value3 > value2);
    REQUIRE(d1 >= value2);
    REQUIRE(getFromStream(value1) == "2022");
    REQUIRE(getFromStream(value2) == "2022-04-25");
    REQUIRE(getFromStream(value3) == getFromStream(value4));

    SECTION("date range check") {
        for (int year = 2010; year <= 2030; year++) {
            std::string yearDate = fmt::format("{}", year);
            auto yearDateDtv     = bsc::fromString<bsc::DateTimeValue>(yearDate);
            REQUIRE(getFromStream(yearDateDtv) == yearDate);
            for (int month = 1; month <= 12; month++) {
                std::string monthDate = fmt::format("{0:}-{1:02d}", year, month);
                auto monthDateDtv     = bsc::fromString<bsc::DateTimeValue>(monthDate);
                REQUIRE(monthDateDtv == yearDateDtv);
                REQUIRE(getFromStream(monthDateDtv) == monthDate);
                for (int day = 1; day <= 28; day++) {
                    std::string fullDate = fmt::format("{0:}-{1:02d}-{2:02d}", year, month, day);
                    std::cout << "Testing " + fullDate << "\n";
                    auto fullDateDtv = bsc::fromString<bsc::DateTimeValue>(fullDate);
                    REQUIRE(monthDateDtv == fullDateDtv);
                    REQUIRE(yearDateDtv == fullDateDtv);
                    REQUIRE(getFromStream(fullDateDtv) == fullDate);
                }
            }
        }
    }
}