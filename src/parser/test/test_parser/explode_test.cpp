//
// Created by Krzysztof Tulidowicz on 26.01.2021.
//
#include <catch2/catch.hpp>
#include <map>
#include <parser/parser/explode.h>
#include <string>
#include <vector>
using namespace bsc;
TEST_CASE("Explode test") {
    std::map<std::string, std::vector<std::string>> dotTestCases = {{"", {}},
                                                                    {"a", {"a"}},
                                                                    {"a.b", {"a", "b"}},
                                                                    {"a.b.c", {"a", "b", "c"}}};

    for (const auto& [string, exploded] : dotTestCases) {
        auto&& result = explode(string, '.');
        REQUIRE(exploded == result);
    }
}