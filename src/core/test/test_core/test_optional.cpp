//
// Created by Krzysztof Tulidowicz on 11.02.2021.
//

#include "core/optional/optional.h"
#include <catch2/catch.hpp>

using namespace bsc;

TEST_CASE("Optional map") {
    using namespace bsc::optional;
    std::optional<int> optInt = 5;
    auto timesTwo             = [](auto i) { return i * 2; };
    auto toString             = [](auto i) { return std::to_string(i); };

    SECTION("*2") {
        auto result = optInt | map(timesTwo);
        REQUIRE(result.has_value() == true);
        REQUIRE(result.value() == 10);
    }

    SECTION("to string") {
        auto result = optInt | map(toString);
        REQUIRE(result.has_value() == true);
        REQUIRE(result.value() == "5");
    }

    SECTION("*2 -> to string") {
        auto result = optInt | map(timesTwo) | map(toString);
        REQUIRE(result.has_value() == true);
        REQUIRE(result.value() == "10");
    }
}

TEST_CASE("Optional if present") {
    using namespace bsc::optional;
    std::optional<int> optInt = 5;

    int result                = 5;
    auto timesTwo             = [&result](auto i) { result = i * 2; };
    optInt | ifPresent(timesTwo);
    REQUIRE(result == 10);
}

TEST_CASE("Optional filter") {
    using namespace bsc::optional;
    auto isEven               = [](auto i) { return i % 2 == 0; };

    SECTION("filter true") {
        std::optional<int> optInt = 4;
        auto result = optInt | filter(isEven);
        REQUIRE(result.has_value() == true);
        REQUIRE(result.value() == 4);
    }

    SECTION("filter false") {
        std::optional<int> optInt = 5;
        auto result = optInt | filter(isEven);
        REQUIRE(result.has_value() == false);
    }
}

TEST_CASE("Optional flatMap") {
    using namespace bsc::optional;
    std::optional<int> optInt = 5;
    auto timesTwo             = [](auto i) { return std::optional(i * 2); };
    auto toString             = [](auto i) { return std::optional(std::to_string(i)); };

    SECTION("*2") {
        auto result = optInt | flatMap(timesTwo);
        REQUIRE(result.has_value() == true);
        REQUIRE(result.value() == 10);
    }

    SECTION("to string") {
        auto result = optInt | flatMap(toString);
        REQUIRE(result.has_value() == true);
        REQUIRE(result.value() == "5");
    }

    SECTION("*2 -> to string") {
        auto result = optInt | flatMap(timesTwo) | flatMap(toString);
        REQUIRE(result.has_value() == true);
        REQUIRE(result.value() == "10");
    }

    SECTION("other") {
        std::optional<std::string> optBadString = "not a number";
        std::optional<std::string> optGoodString = "10";

        auto toInt = [](auto i){
            try {
                return std::make_optional<int>(std::stoi(i));
            } catch (const std::invalid_argument&) {
                return std::optional<int>{};
            }
        };

        auto resultBad = optBadString | flatMap(toInt);
        auto resultGood = optGoodString | flatMap(toInt);

        REQUIRE(resultBad.has_value() == false);
        REQUIRE(resultGood.has_value() == true);
        REQUIRE(resultGood.value() == 10);
    }
}

TEST_CASE("Optional orElse") {
    using namespace bsc::optional;
    SECTION("value present") {
        std::optional<int> optInt = 5;
        auto result               = optInt | orElse(111);
        REQUIRE(result == 5);
    }

    SECTION("empty optional") {
        std::optional<int> optInt;
        auto result = optInt | orElse(111);
        REQUIRE(result == 111);
    }
}

TEST_CASE("Optional orElseThrow") {
    using namespace bsc::optional;
    SECTION("value present") {
        std::optional<int> optInt = 5;
        auto result               = optInt | orElseThrow(std::domain_error("Error"));
        REQUIRE(result == 5);
    }

    SECTION("empty optional") {
        std::optional<int> optInt;
        REQUIRE_THROWS_AS(optInt | orElseThrow(std::domain_error("Error")), std::domain_error);
    }
}

TEST_CASE("Optional orElseGet") {
    using namespace bsc::optional;
    auto else111 = [] { return 111; };
    SECTION("value present") {
        std::optional<int> optInt = 5;
        auto result               = optInt | orElseGet(else111);
        REQUIRE(result == 5);
    }

    SECTION("empty optional") {
        std::optional<int> optInt;
        auto result = optInt | orElseGet(else111);
        REQUIRE(result == 111);
    }
}

TEST_CASE("Optional concept") {
    if constexpr (optional::IsOptional<std::optional<int>>) {
        REQUIRE(true);
    } else {
        REQUIRE(false);
    }

    if constexpr (optional::IsOptional<float>) {
        REQUIRE(false);
    } else {
        REQUIRE(true);
    }
}
