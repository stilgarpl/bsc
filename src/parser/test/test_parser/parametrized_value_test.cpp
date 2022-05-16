//
// Created by Krzysztof Tulidowicz on 01.02.22.
//

#include "constant/standard/World.h"
#include "math/arithmetic.h"
#include "parser/value/DateTimeValue.h"
#include "parser/value/ModifierAffixes.h"
#include "parser/value/ParametrizedValue.h"
#include "parser/value/SiAffix.h"
#include "parser/value/StandardAffixes.h"
#include <catch2/catch.hpp>
#include <map>
#include <string>
using namespace bsc;
TEST_CASE("Parametrized value test") {

    SECTION("noaffix int ") {
        ParametrizedValue<int, affixes::NoAffix, affixes::NoAffix> value{"5"};
        REQUIRE(value.getRawValue().has_value() == true);
        REQUIRE(value.getRawValue().value() == 5);
    }

    SECTION("sign int") {
        SECTION("no sign") {
            ParametrizedValue<int, affixes::SignAffix, affixes::SignAffix> value{"5"};
            REQUIRE(value.getRawValue().has_value() == true);
            REQUIRE(value.getRawValue().value() == 5);
            REQUIRE(value.getPrefix().has_value() == false);
        }

        SECTION("with sign") {
            ParametrizedValue<int, affixes::SignAffix, affixes::NoAffix> value{"+5"};
            REQUIRE(value.getRawValue().has_value() == true);
            REQUIRE(value.getRawValue().value() == 5);
            REQUIRE(value.getPrefix().has_value() == true);
            REQUIRE(value.getPrefix().value() == "+");
        }
    }

    SECTION("relation int") {
        SECTION("no relation") {
            ParametrizedValue<int, affixes::RelationAffix, affixes::NoAffix> value{"5"};
            REQUIRE(value.getRawValue().has_value() == true);
            REQUIRE(value.getRawValue().value() == 5);
            REQUIRE(value.getPrefix().has_value() == false);
            REQUIRE(value.compare(5) == true);
            REQUIRE(value.compare(4) == false);
            REQUIRE(value.compare(6) == false);
        }
        SECTION("> relation") {
            ParametrizedValue<int, affixes::RelationAffix, affixes::NoAffix> value{">5"};
            REQUIRE(value.getRawValue().has_value() == true);
            REQUIRE(value.getRawValue().value() == 5);
            REQUIRE(value.getPrefix().has_value() == true);
            REQUIRE(value.getPrefix().value() == ">");
            REQUIRE(value.compare(5) == false);
            REQUIRE(value.compare(4) == false);
            REQUIRE(value.compare(6) == true);
        }
        SECTION("> < relation") {
            ParametrizedValue<int, affixes::RelationAffix, affixes::RelationAffix> value{">5<"};
            REQUIRE(value.getRawValue().has_value() == true);
            REQUIRE(value.getRawValue().value() == 5);
            REQUIRE(value.getPrefix().has_value() == true);
            REQUIRE(value.getPrefix().value() == ">");
            REQUIRE(value.getPostfix().has_value() == true);
            REQUIRE(value.getPostfix().value() == "<");
            REQUIRE(value.compare(5) == false);
            REQUIRE(value.compare(4) == false);
            REQUIRE(value.compare(6) == true);
        }
        SECTION("> > relation") {
            ParametrizedValue<int, affixes::RelationAffix, affixes::RelationAffix> value{"<5<"};
            REQUIRE(value.getRawValue().has_value() == true);
            REQUIRE(value.getRawValue().value() == 5);
            REQUIRE(value.getPrefix().has_value() == true);
            REQUIRE(value.getPrefix().value() == "<");
            REQUIRE(value.getPostfix().has_value() == true);
            REQUIRE(value.getPostfix().value() == "<");
            REQUIRE(value.compare(5) == false);
            REQUIRE(value.compare(4) == true);
            REQUIRE(value.compare(6) == true);
        }
    }

    SECTION("SI int") {
        SECTION("with suffix") {
            ParametrizedValue<int, affixes::NoAffix, affixes::SiEnumPrefix> value("5k");
            REQUIRE(value.getRawValue().has_value() == true);
            REQUIRE(value.getRawValue().value() == 5);
            REQUIRE(value.getPostfix().has_value() == true);
            REQUIRE(value.getPostfix().value() == SiPrefix::k);
            REQUIRE(value.getValue() == 5000);
        }
        SECTION("without suffix") {
            ParametrizedValue<int, affixes::NoAffix, affixes::SiEnumPrefix> value("5");
            REQUIRE(value.getRawValue().has_value() == true);
            REQUIRE(value.getRawValue().value() == 5);
            REQUIRE(value.getPostfix().has_value() == true); //@todo I'd prefer if this didn't have value, but I need default value for neutral transform
            REQUIRE(value.getPostfix().value() == SiPrefix::null);
            REQUIRE(value.getValue() == 5);
        }
    }

    SECTION("double SI int") {

        ParametrizedValue<int, affixes::SiEnumPrefix, affixes::SiEnumPrefix> value("k5k");
        REQUIRE(value.getRawValue().has_value() == true);
        REQUIRE(value.getRawValue().value() == 5);
        REQUIRE(value.getPrefix().has_value() == true);
        REQUIRE(value.getPrefix().value() == SiPrefix::k);
        REQUIRE(value.getPostfix().has_value() == true);
        REQUIRE(value.getPostfix().value() == SiPrefix::k);
        REQUIRE(value.getValue() == 5000000);
    }

    SECTION("SI int relation") {

        ParametrizedValue<int, affixes::RelationAffix, affixes::SiEnumPrefix> value(">=5k");
        REQUIRE(value.getRawValue().has_value() == true);
        REQUIRE(value.getRawValue().value() == 5);
        REQUIRE(value.getPrefix().has_value() == true);
        REQUIRE(value.getPrefix().value() == ">=");
        REQUIRE(value.getPostfix().has_value() == true);
        REQUIRE(value.getPostfix().value() == SiPrefix::k);
        REQUIRE(value.getValue() == 5000);
        REQUIRE(value.compare(100) == false);
        REQUIRE(value.compare(4999) == false);
        REQUIRE(value.compare(5000) == true);
        REQUIRE(value.compare(900000) == true);
    }

    SECTION("DateTime relation") {
        ParametrizedValue<DateTimeValue, affixes::RelationAffix, affixes::NoAffix> value(">2020-05");
        REQUIRE(value.getRawValue().has_value() == true);
        REQUIRE(value.getRawValue().value() == fromString<DateTimeValue>("2020-05"));
        REQUIRE(value.getPrefix().has_value() == true);
        REQUIRE(value.getPrefix().value() == ">");
        REQUIRE(value.getValue() ==  fromString<DateTimeValue>("2020-05"));
        REQUIRE(value.compare(fromString<DateTimeValue>("2020-05")) == false);
        REQUIRE(value.compare(fromString<DateTimeValue>("2020-05-28")) == false);
        REQUIRE(value.compare(fromString<DateTimeValue>("2020-06-01")) == true);
        REQUIRE(value.compare(fromString<DateTimeValue>("2021")) == true);
        REQUIRE(value.compare(fromString<DateTimeValue>("2020")) == false);
        REQUIRE(value.compare(fromString<DateTimeValue>("2019")) == false);

    }
}

TEST_CASE("Affixes test") {

    SECTION("si enum") {
        affixes::EnumAffix<SiPrefix> siEnumAffix;
        REQUIRE(siEnumAffix.pattern == "(?:y|z|a|f|p|n|u|m|c|d|null|da|h|k|M|G|T|P|E|Z|Y)");
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
        auto identity = [](auto v, auto i) { return i; };
        affixes::TransformAffix<affixes::EnumAffix<SiPrefix>, identity> transformAffix;
        transformAffix.value = SiPrefix::G;
        REQUIRE(transformAffix.transform(118) == 118);
        REQUIRE(transformAffix.transform(0) == 0);
        REQUIRE(transformAffix.transform(2.0) == 2.0);
    }
    SECTION("replace value") {
        auto replace = [](auto v, auto i) { return v; };
        affixes::TransformAffix<affixes::EnumAffix<SiPrefix>, replace> transformAffix;
        transformAffix.value = SiPrefix::G;
        REQUIRE(transformAffix.transform(118) == SiPrefix::G);
        REQUIRE(transformAffix.transform(0) == SiPrefix::G);
        REQUIRE(transformAffix.transform(2.0) == SiPrefix::G);
    }
    SECTION("add underlying value") {
        auto add = [](auto v, auto i) { return static_cast<std::underlying_type_t<decltype(v)>>(v) + i; };
        affixes::TransformAffix<affixes::EnumAffix<SiPrefix>, add> transformAffix;
        transformAffix.value = SiPrefix::G;
        REQUIRE(transformAffix.transform(118) == 127);
        REQUIRE(transformAffix.transform(0) == 9);
        REQUIRE(transformAffix.transform(2.0) == 11.0);
    }
    SECTION("multiply") {
        auto multiply = [](auto v, auto i) { return static_cast<std::underlying_type_t<decltype(v)>>(v) * i; };
        affixes::TransformAffix<affixes::EnumAffix<SiPrefix>, multiply> transformAffix;
        transformAffix.value = SiPrefix::G;
        REQUIRE(transformAffix.transform(118) == 118 * 9);
        REQUIRE(transformAffix.transform(0) == 0);
        REQUIRE(transformAffix.transform(2.0) == 18.0);
    }
    SECTION("multiply pown") {
        auto multiplyPown = []<typename EnumType>(EnumType v, auto i) {
            return math::pown(10ll, static_cast<std::underlying_type_t<EnumType>>(v)) * i;
        };
        affixes::TransformAffix<affixes::EnumAffix<SiPrefix>, multiplyPown> transformAffix;
        transformAffix.value = SiPrefix::G;
        REQUIRE(transformAffix.transform(118) == 118000000000);
        REQUIRE(transformAffix.transform(0) == 0);
        REQUIRE(transformAffix.transform(2.0L) == 2000000000.0);
    }
}

TEST_CASE("RelationAffix test") {

    affixes::RelationAffix relationAffix;
    SECTION("<=") {
        relationAffix.value = "<=";
        REQUIRE(relationAffix.compare(118, 120) == true);
        REQUIRE(relationAffix.compare(0, 0) == true);
        REQUIRE(relationAffix.compare(2.0, 1) == false);
        REQUIRE(relationAffix.compare(2.0, 3.0) == true);
        REQUIRE(relationAffix.compare(2.0, 2) == true);
    }
    SECTION("==") {
        relationAffix.value = "=";
        REQUIRE(relationAffix.compare(118, 120) == false);
        REQUIRE(relationAffix.compare(0, 0) == true);
        REQUIRE(relationAffix.compare(2.0, 1) == false);
        REQUIRE(relationAffix.compare(2.0, 3.0) == false);
        REQUIRE(relationAffix.compare(2.0, 2) == true);
    }
    SECTION("!=") {
        relationAffix.value = "!=";
        REQUIRE(relationAffix.compare(118, 120) == true);
        REQUIRE(relationAffix.compare(0, 0) == false);
        REQUIRE(relationAffix.compare(2.0, 1) == true);
        REQUIRE(relationAffix.compare(2.0, 3.0) == true);
        REQUIRE(relationAffix.compare(2.0, 2) == false);
    }
}

TEST_CASE("Affix concepts") {
    constexpr auto multiplyPown = [](auto v, auto i) { return math::pown(10ll, static_cast<std::underlying_type_t<decltype(v)>>(v)) * i; };

    using SiAffix               = affixes::TransformAffix<affixes::EnumAffix<SiPrefix>, multiplyPown>;

    REQUIRE(affixes::IsAffix<int> == false);
    REQUIRE(affixes::IsAffix<bool> == false);
    REQUIRE(affixes::IsAffix<affixes::NoAffix> == true);
    REQUIRE(affixes::IsAffix<affixes::SignAffix> == true);
    REQUIRE(affixes::IsAffix<SiAffix> == true);

    REQUIRE(affixes::IsRelationalAffix<int, int> == false);
    REQUIRE(affixes::IsRelationalAffix<bool, bool> == false);
    REQUIRE(affixes::IsRelationalAffix<affixes::NoAffix, int> == false);
    REQUIRE(affixes::IsRelationalAffix<affixes::SignAffix, int> == false);
    REQUIRE(affixes::IsRelationalAffix<SiAffix, int> == false);

    REQUIRE(affixes::IsValueTransformAffix<int, int> == false);
    REQUIRE(affixes::IsValueTransformAffix<bool, bool> == false);
    REQUIRE(affixes::IsValueTransformAffix<affixes::NoAffix, int> == false);
    REQUIRE(affixes::IsValueTransformAffix<affixes::SignAffix, int> == true);
    REQUIRE(affixes::IsValueTransformAffix<SiAffix, int> == true);

    REQUIRE(affixes::IsValueAffix<int> == false);
    REQUIRE(affixes::IsValueAffix<bool> == false);
    REQUIRE(affixes::IsValueAffix<affixes::NoAffix> == false);
    REQUIRE(affixes::IsValueAffix<affixes::SignAffix> == true);
    REQUIRE(affixes::IsValueAffix<SiAffix> == true);
}

