//
// Created by Krzysztof Tulidowicz on 14.11.2019.
//

#include <catch2/catch.hpp>
#include <core/utils/crypto.h>
using namespace bsc;
TEST_CASE("Sha1 test") {

    std::string sha1 = calculateSha1OfString("TEST STRING");
    REQUIRE_THAT(sha1, Catch::Matchers::Equals("d39d009c05797a93a79720952e99c7054a24e7c4"));

}