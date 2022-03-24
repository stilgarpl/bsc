//
// Created by stilgar on 13.10.2020.
//
#include <catch2/catch.hpp>
#include <math/arithmetic.h>

using namespace bsc;

TEST_CASE("pown test") {

    struct TestData {
        long long base;
        unsigned exponent;
        long long expectedResult;
    };

    std::vector<TestData> testData = {{1, 0, 1},
                                      {2, 0, 1},
                                      {1, 1, 1},
                                      {2, 1, 2},
                                      {10, 0, 1},
                                      {10, 1, 10},
                                      {10, 2, 100},
                                      {10, 4, 10000},
                                      {10, 5, 100000},
                                      {100, 2, 10000},
                                      {10, 10, 10000000000}};

    for (const auto& datum : testData) {
        REQUIRE(math::pown(datum.base, datum.exponent) == datum.expectedResult);
    }
}
