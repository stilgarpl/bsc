//
// Created by stilgar on 13.10.2020.
//
#include <catch2/catch.hpp>
#include <core/auto/AutoCaller.h>
using namespace bsc;

bool autoTested = false;

void autoCallerTestFunc() { autoTested = true; }

class AutoCallerTest : protected AutoCaller<AutoCallerTest, autoCallerTestFunc> {

public:
    explicit AutoCallerTest(int i) : AutoCaller(i) {}
};

TEST_CASE("AutoCaller test") { REQUIRE(autoTested == true); }