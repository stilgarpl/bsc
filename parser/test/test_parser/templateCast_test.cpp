//
// Created by stilgar on 15.01.2020.
//

#include <catch2/catch.hpp>
#include <string>
#include <vector>
#include <parser/parser/explode.h>
#include <parser/cast/templateCast.h>



TEST_CASE("Template Cast Test") {
    struct testStruct {
        int integer = 0;
        std::string string = "";
        std::pair<std::string, int> pair = std::make_pair("", 0);
        std::vector<int> vectorOfInts{};
    } testResult;

    std::string testString = "12 test_string b=5 1,2,3,4,5";
    std::function func = [&testResult](int i, std::string s, std::pair<std::string, int> p, std::vector<int> v) {
        testResult.integer = i;
        testResult.string = s;
        testResult.pair = p;
        testResult.vectorOfInts = v;
    };

    bsc::runStandardFunction<void, int, std::string, std::pair<std::string, int>, std::vector<int>>(func, bsc::explode(
            testString,
            ' '));

    REQUIRE(testResult.integer == 12);
    REQUIRE(testResult.string == "test_string");
    REQUIRE(testResult.pair.first == "b");
    REQUIRE(testResult.pair.second == 5);
    REQUIRE(testResult.vectorOfInts.size() == 5);
    REQUIRE(testResult.vectorOfInts[0] == 1);
    REQUIRE(testResult.vectorOfInts[1] == 2);
    REQUIRE(testResult.vectorOfInts[2] == 3);
    REQUIRE(testResult.vectorOfInts[3] == 4);
    REQUIRE(testResult.vectorOfInts[4] == 5);


}