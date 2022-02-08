//
// Created by stilgar on 13.10.2020.
//
#include <catch2/catch.hpp>
#include <constant/standard/World.h>

using namespace bsc;


TEST_CASE("SiPrefix test") {
    //@todo stupid test
    REQUIRE(static_cast<int>(SiPrefix::k) == 3);
}
