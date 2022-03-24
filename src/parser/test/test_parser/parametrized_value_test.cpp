//
// Created by Krzysztof Tulidowicz on 01.02.22.
//

#include "constant/standard/World.h"
#include "parser/value/StandardAffixes.h"
#include "parser/value/ParametrizedValue.h"
#include <catch2/catch.hpp>
#include <map>
#include <string>
#include "math/arithmetic.h"
using namespace bsc;
TEST_CASE("Parametrized value test") {

    SECTION("noaffix int ") {
        ParametrizedValue<int, affixes::NoAffix, affixes::NoAffix> value{"5"};
        REQUIRE(value.getValue().has_value() == true);
        REQUIRE(value.getValue().value() == 5);
    }

    SECTION("sign int") {
        SECTION("no sign") {
            ParametrizedValue<int, affixes::SignAffix, affixes::SignAffix> value{"5"};
            REQUIRE(value.getValue().has_value() == true);
            REQUIRE(value.getValue().value() == 5);
            REQUIRE(value.getPrefix().has_value() == false);
        }

        SECTION("with sign") {
            ParametrizedValue<int, affixes::SignAffix, affixes::NoAffix> value{"+5"};
            REQUIRE(value.getValue().has_value() == true);
            REQUIRE(value.getValue().value() == 5);
            REQUIRE(value.getPrefix().has_value() == true);
            REQUIRE(value.getPrefix().value() == "+");
        }
    }

    SECTION("SI int") {
        using SiEnumPrefix =  affixes::EnumAffix<SiPrefix>;

        ParametrizedValue<int, affixes::NoAffix, SiEnumPrefix> value("5k") ;
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

    SECTION("sign affix") {
        affixes::SignAffix signAffix;
        SECTION("+") {
            signAffix.value = "+";
            REQUIRE(signAffix.transform(5) == 5);
        }
        SECTION("empty") {
            REQUIRE(signAffix.transform(5) == 5);
        }
        SECTION("-") {
            signAffix.value = "-";
            REQUIRE(signAffix.transform(5) == -5);
            REQUIRE(signAffix.transform(-15) == 15);
        }
    }
}

TEST_CASE("TransformAffix test") {
    //@todo C++23 to_underlying
        SECTION("identity") {
            auto identity = [] (auto i, auto v) {return i;};
            affixes::TransformAffix<affixes::EnumAffix<SiPrefix>, identity> transformAffix;
            transformAffix.value = SiPrefix::G;
            REQUIRE(transformAffix.transform(118) == 118);
            REQUIRE(transformAffix.transform(0) == 0);
            REQUIRE(transformAffix.transform(2.0) == 2.0);
        }
        SECTION("replace value") {
            auto replace = [] (auto i, auto v) {return v;};
            affixes::TransformAffix<affixes::EnumAffix<SiPrefix>, replace> transformAffix;
            transformAffix.value = SiPrefix::G;
            REQUIRE(transformAffix.transform(118) == SiPrefix::G);
            REQUIRE(transformAffix.transform(0) == SiPrefix::G);
            REQUIRE(transformAffix.transform(2.0) == SiPrefix::G);
        }
        SECTION("add underlying value") {
            auto add = [] (auto i, auto v) {return static_cast<std::underlying_type_t<decltype(v)>>(v)+i;};
            affixes::TransformAffix<affixes::EnumAffix<SiPrefix>, add> transformAffix;
            transformAffix.value = SiPrefix::G;
            REQUIRE(transformAffix.transform(118) == 127);
            REQUIRE(transformAffix.transform(0) == 9);
            REQUIRE(transformAffix.transform(2.0) == 11.0);
        }
        SECTION("multiply") {
            auto multiply = [] (auto i, auto v) {return static_cast<std::underlying_type_t<decltype(v)>>(v)*i;};
            affixes::TransformAffix<affixes::EnumAffix<SiPrefix>, multiply> transformAffix;
            transformAffix.value = SiPrefix::G;
            REQUIRE(transformAffix.transform(118) == 118*9);
            REQUIRE(transformAffix.transform(0) == 0);
            REQUIRE(transformAffix.transform(2.0) == 18.0);
        }
        SECTION("multiply pown") {
            auto multiplyPown = [] (auto i, auto v) {return math::pown(10ll,static_cast<std::underlying_type_t<decltype(v)>>(v))*i;};
            affixes::TransformAffix<affixes::EnumAffix<SiPrefix>, multiplyPown> transformAffix;
            transformAffix.value = SiPrefix::G;
            REQUIRE(transformAffix.transform(118) == 118000000000);
            REQUIRE(transformAffix.transform(0) == 0);
            REQUIRE(transformAffix.transform(2.0) == 2000000000.0);
        }
}

TEST_CASE("Affix concepts") {
    REQUIRE(Affix<int> == false);
    REQUIRE(Affix<bool> == false);
    REQUIRE(Affix<affixes::NoAffix> == true);
    REQUIRE(Affix<affixes::SignAffix> == true);

    REQUIRE(affixes::IsRelationalAffix<int, int> == false);
    REQUIRE(affixes::IsRelationalAffix<bool, bool> == false);
    REQUIRE(affixes::IsRelationalAffix<affixes::NoAffix, int> == false);
    REQUIRE(affixes::IsRelationalAffix<affixes::SignAffix, int > == false);

    REQUIRE(affixes::IsValueTransformAffix<int, int> == false);
    REQUIRE(affixes::IsValueTransformAffix<bool, bool> == false);
    REQUIRE(affixes::IsValueTransformAffix<affixes::NoAffix, int> == false);
    REQUIRE(affixes::IsValueTransformAffix<affixes::SignAffix, int > == false);

    REQUIRE(affixes::IsValueAffix<int> == false);
    REQUIRE(affixes::IsValueAffix<bool> == false);
    REQUIRE(affixes::IsValueAffix<affixes::NoAffix> == false);
    REQUIRE(affixes::IsValueAffix<affixes::SignAffix> == true);

    //@todo add tests for transforming affix with si and multiply pown
}