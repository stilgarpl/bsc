//
// Created by stilgar on 13.12.2019.
//

#include <catch2/catch.hpp>
#include <core/generator/SequenceAlgorithm.h>
#include <core/generator/Generator.h>
#include <core/generator/RandomAlgorithm.h>
#include <core/generator/RandomHashStringAlgorithm.h>



TEST_CASE("Sequence generator test") {

    bsc::Generator<bsc::SequenceAlgorithm<int>> generator;

    generator.reset(0);
    REQUIRE(generator.nextValue() == 1);
    REQUIRE(generator.nextValue() == 2);
    REQUIRE(generator.nextValue() == 3);

}


TEST_CASE("Random generator test") {

    bsc::Generator<bsc::RandomAlgorithm> generator;

    REQUIRE_NOTHROW(generator.nextValue());
    REQUIRE_NOTHROW(generator.nextValue());
    REQUIRE_NOTHROW(generator.nextValue());


}

TEST_CASE("String hash generator test") {

    bsc::Generator<bsc::RandomHashStringAlgorithm> generator;

    auto value = generator.nextValue();

    REQUIRE(value.length() == 40);

    auto value2 = generator.nextValue();
    REQUIRE(value2.length() == 40);
    REQUIRE(value != value2); //once in a million years this may be true and fail.




}