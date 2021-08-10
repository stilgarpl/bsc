//
// Created by Krzysztof Tulidowicz on 09.08.2021.
//
#include <catch2/catch.hpp>
#include <context/options/Flags.h>
#include <sstream>

using namespace bsc;

enum class OptionsEnum {
    FIRST,
    SECOND,
    THIRD,
    FOURTH,
};

TEST_CASE("Flags test") {

    SECTION("list initialize") {
        Flags<OptionsEnum> options = {OptionsEnum::FIRST, OptionsEnum::THIRD};
        REQUIRE(options.has(OptionsEnum::FIRST));
        REQUIRE(!options.has(OptionsEnum::SECOND));
        REQUIRE(options.has(OptionsEnum::THIRD));
        REQUIRE(!options.has(OptionsEnum::FOURTH));
    }

    SECTION("default initialize") {
        Flags<OptionsEnum> options;
        REQUIRE(!options.has(OptionsEnum::FIRST));
        REQUIRE(!options.has(OptionsEnum::SECOND));
        REQUIRE(!options.has(OptionsEnum::THIRD));
        REQUIRE(!options.has(OptionsEnum::FOURTH));
        SECTION("enable") {
            options.enable(OptionsEnum::FIRST);
            options.enable(OptionsEnum::SECOND);
            REQUIRE(options.has(OptionsEnum::FIRST));
            REQUIRE(options.has(OptionsEnum::SECOND));
            REQUIRE(!options.has(OptionsEnum::THIRD));
            REQUIRE(!options.has(OptionsEnum::FOURTH));
            SECTION("disable") {
                options.disable(OptionsEnum::SECOND);
                REQUIRE(options.has(OptionsEnum::FIRST));
                REQUIRE(!options.has(OptionsEnum::SECOND));
                REQUIRE(!options.has(OptionsEnum::THIRD));
                REQUIRE(!options.has(OptionsEnum::FOURTH));
            }
        }

        SECTION("bulk") {
            SECTION("enable") {
                options.enable({OptionsEnum::FIRST, OptionsEnum::SECOND});
                REQUIRE(options.has(OptionsEnum::FIRST));
                REQUIRE(options.has(OptionsEnum::SECOND));
                REQUIRE(!options.has(OptionsEnum::THIRD));
                REQUIRE(!options.has(OptionsEnum::FOURTH));
            }
            SECTION("disable") {
                options.enable({OptionsEnum::FIRST, OptionsEnum::SECOND, OptionsEnum::THIRD});
                options.disable({OptionsEnum::FIRST, OptionsEnum::SECOND});
                REQUIRE(!options.has(OptionsEnum::FIRST));
                REQUIRE(!options.has(OptionsEnum::SECOND));
                REQUIRE(options.has(OptionsEnum::THIRD));
                REQUIRE(!options.has(OptionsEnum::FOURTH));
            }
        }

        SECTION("operators") {
            SECTION("variant 1") {
                options.enable(OptionsEnum::FIRST);
                REQUIRE(options == (OptionsEnum::FIRST));
                REQUIRE(!(options == OptionsEnum::SECOND));
                REQUIRE(!(options == OptionsEnum::THIRD));
                REQUIRE(!(options == OptionsEnum::FOURTH));
                REQUIRE((options == OptionsEnum::THIRD or OptionsEnum::FIRST));
                REQUIRE(!(options == OptionsEnum::THIRD or OptionsEnum::SECOND));
                REQUIRE(!(options == OptionsEnum::FIRST and OptionsEnum::SECOND));
                REQUIRE((options == OptionsEnum::THIRD and OptionsEnum::SECOND or OptionsEnum::FIRST));
                REQUIRE(!(options == OptionsEnum::THIRD and OptionsEnum::FIRST or OptionsEnum::SECOND));
            }
            SECTION("variant 2") {
                options.enable(OptionsEnum::FIRST);
                REQUIRE(options == (OptionsEnum::FIRST));
                REQUIRE(!(options == OptionsEnum::SECOND));
                REQUIRE(!(options == OptionsEnum::THIRD));
                REQUIRE(!(options == OptionsEnum::FOURTH));
                REQUIRE((options == OptionsEnum::THIRD or OptionsEnum::FIRST));
                REQUIRE(!(options == OptionsEnum::THIRD or OptionsEnum::SECOND));
                REQUIRE(!(options == OptionsEnum::FIRST and OptionsEnum::SECOND));
                REQUIRE((options == OptionsEnum::THIRD and OptionsEnum::SECOND or OptionsEnum::FIRST));
                REQUIRE(!(options == OptionsEnum::THIRD and OptionsEnum::FIRST or OptionsEnum::SECOND));
            }
        }
    }

    SECTION("Logic") {
        Flags<OptionsEnum> options = {OptionsEnum::FIRST, OptionsEnum::SECOND};
        REQUIRE(options.has(OptionsEnum::FIRST));
        REQUIRE(options.has(OptionsEnum::SECOND));
        REQUIRE(!options.has(OptionsEnum::THIRD));
        REQUIRE(!options.has(OptionsEnum::FOURTH));
        SECTION("and") {
            REQUIRE(options.has(OptionsEnum::FIRST).andHas(OptionsEnum::SECOND));
            REQUIRE(!options.has(OptionsEnum::SECOND).andHas(OptionsEnum::THIRD));
            REQUIRE(!options.has(OptionsEnum::THIRD).andHas(OptionsEnum::SECOND));
            REQUIRE(!options.has(OptionsEnum::FOURTH).andHas(OptionsEnum::SECOND));
            REQUIRE(options.has(OptionsEnum::FIRST).andHas(OptionsEnum::FIRST));
        }
        SECTION("or") {
            REQUIRE(options.has(OptionsEnum::FIRST).orHas(OptionsEnum::SECOND));
            REQUIRE(options.has(OptionsEnum::SECOND).orHas(OptionsEnum::THIRD));
            REQUIRE(options.has(OptionsEnum::THIRD).orHas(OptionsEnum::SECOND));
            REQUIRE(!options.has(OptionsEnum::FOURTH).orHas(OptionsEnum::THIRD));
            REQUIRE(options.has(OptionsEnum::FIRST).orHas(OptionsEnum::FIRST));
        }
        SECTION("negation") {
            REQUIRE(!(options != OptionsEnum::FIRST));
            REQUIRE(!(options != OptionsEnum::SECOND));
            REQUIRE(options != OptionsEnum::THIRD);
            REQUIRE(options != OptionsEnum::FOURTH);
            REQUIRE(!(options == OptionsEnum::FOURTH or OptionsEnum::THIRD));
            REQUIRE(!(options == OptionsEnum::FOURTH and OptionsEnum::THIRD));
            REQUIRE((options != OptionsEnum::FOURTH and OptionsEnum::THIRD));
            REQUIRE((options != OptionsEnum::FOURTH or OptionsEnum::THIRD));
            REQUIRE((options != OptionsEnum::FOURTH and OptionsEnum::FIRST));
            REQUIRE(!(options != OptionsEnum::FOURTH or OptionsEnum::FIRST));
        }
    }

}