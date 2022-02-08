//
// Created by Krzysztof Tulidowicz on 01.02.22.
//

#include "constant/standard/World.h"
#include "parser/value/Affixes.h"
#include "parser/value/ParametrizedValue.h"
#include <catch2/catch.hpp>
#include <map>
#include <parser/parser/explode.h>
#include <string>
#include <vector>
using namespace bsc;
TEST_CASE("Parametrized value test") {

    SECTION("noaffix int ") {
        ParametrizedValue<int, affixes::noAffix, affixes::noAffix> value{"5"};
        REQUIRE(value.getValue().has_value() == true);
        REQUIRE(value.getValue().value() == 5);
    }

    SECTION("sign int") {
        SECTION("no sign") {
            ParametrizedValue<int, affixes::signAffix, affixes::noAffix> value{"5"};
            REQUIRE(value.getValue().has_value() == true);
            REQUIRE(value.getValue().value() == 5);
            REQUIRE(value.getPrefix().has_value() == false);
        }

        SECTION("with sign") {
            ParametrizedValue<int, affixes::signAffix, affixes::noAffix> value{"+5"};
            REQUIRE(value.getValue().has_value() == true);
            REQUIRE(value.getValue().value() == 5);
            REQUIRE(value.getPrefix().has_value() == true);
            REQUIRE(value.getPrefix().value() == "+");
        }
    }

    SECTION("SI int") {
        constexpr affixes::EnumAffix<SiPrefix> siEnumAffix;

        ParametrizedValue<int, affixes::noAffix, siEnumAffix> value("5k") ;
        REQUIRE(value.getValue().has_value() == true);
        REQUIRE(value.getValue().value() == 5);
        REQUIRE(value.getPostfix().has_value() == true);
        REQUIRE(value.getPostfix().value() == SiPrefix::k);
    }

}

TEST_CASE("Affixes test") {

    SECTION("si enum") {
        affixes::EnumAffix<SiPrefix> siEnumAffix;
        REQUIRE(siEnumAffix.pattern == "(y|z|a|f|p|n|u|m|c|d|da|h|k|M|G|T|P|E|Z|Y)");
    }

}