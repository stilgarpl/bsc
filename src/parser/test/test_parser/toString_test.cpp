//
// Created by Krzysztof Tulidowicz on 14.06.2021.
//
#include <catch2/catch.hpp>
#include <map>
#include <parser/writer/toString.h>
#include <list>

using namespace bsc;

TEST_CASE("ToString writer test") {
    Writer writer;

    SECTION("string to string") {
        SECTION("char array") {
            //given
            const char * testString = "test string";
            //when
            auto result = writer.toString(testString);
            //then
            REQUIRE(result == "test string");
        }

        SECTION("std::string to string") {
            //given
            std::string testString = "test string";
            //when
            auto result = writer.toString(testString);
            //then
            REQUIRE(result == "test string");
        }

        SECTION("std::string_view to string") {
            //given
            std::string_view testString = "test string";
            //when
            auto result = writer.toString(testString);
            //then
            REQUIRE(result == "test string");
        }
    }

    SECTION("integers") {
        SECTION("int") {
            //given
            int testInt = 5;
            //when
            auto result = writer.toString(testInt);
            //then
            REQUIRE(result == "5");
        }
        SECTION("const int") {
            //given
            const int testInt = 15;
            //when
            auto result = writer.toString(testInt);
            //then
            REQUIRE(result == "15");
        }
        SECTION("long") {
            //given
            long testInt = 1234567890;
            //when
            auto result = writer.toString(testInt);
            //then
            REQUIRE(result == "1234567890");
        }
        SECTION("char") {
            //given
            char testInt = 6;
            //when
            auto result = writer.toString(testInt);
            //then
            REQUIRE(result == "6");
        }
    }

    SECTION("floating points") {
        SECTION("float") {
            // given
            float testInt = 2.0;
            // when
            auto result = writer.toString(testInt);
            // then
            REQUIRE(result == "2.000000");
        }
        SECTION("double") {
            // given
            double testInt = 4.0;
            // when
            auto result = writer.toString(testInt);
            // then
            REQUIRE(result == "4.000000");
        }
    }

    SECTION("containers") {
        SECTION("list of ints") {
            //given
            std::list<int> ints = {1,4,3,6};
            //when
            auto result = writer.toString(ints);
            //then
            REQUIRE(result == "1436");
        }
    }
}