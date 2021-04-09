//
// Created by stilgar on 13.10.2020.
//
#include <catch2/catch.hpp>
#include <context/auto/AutoCaller.h>
#include <context/context/AutoContextSetter.h>
#include <context/context/Context.h>
using namespace bsc;

bool autoTested = false;

void autoCallerTestFunc() {
    autoTested = true;
}

class AutoCallerTest : protected AutoCaller<AutoCallerTest, autoCallerTestFunc> {

public:
    explicit AutoCallerTest(int i) : AutoCaller(i) {
    }
};

class TestContext : public AutoContextSetter<TestContext> {
public:
    TestContext(){};
};

TEST_CASE("AutoCaller test") {
    REQUIRE(autoTested == true);
}

TEST_CASE("AutoContextSetter test") {
    auto contextPtr = bsc::Context::makeContext(true);

    REQUIRE(contextPtr->has<TestContext>() == true);
}