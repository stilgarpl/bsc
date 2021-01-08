//
// Created by Krzysztof Tulidowicz on 30.10.2020.
//

#include <catch2/catch.hpp>
#include <core/text/TextUtils.h>

using namespace bsc;

TEST_CASE("escapeAllRegexCharacters test") {
    std::string text         = "[test(string).]";
    std::string expectedText = R"(\[test\(string\)\.\])";
    auto result              = TextUtils::escapeAllRegexCharacters(text);
    REQUIRE(result == expectedText);
}
