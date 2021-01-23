//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//
#include <catch2/catch.hpp>
#include <context/context/Context.h>
#include <properties/Property.h>
#include <properties/PropertyContext.h>
#include <properties/PropertyLoader.h>
using namespace bsc;

TEST_CASE("Simple property test ") {
    //    auto context = Context::makeContext();
    //    Context::setActiveContext(context);
    //
    //    PropertyLoader::parseToContext("test: 7\ntest5: 5\ntestOuter:\n  testInner: 88\n");
    //
    //    Property<"test"> a;
    //    Property<"test5",int> simpleInt;
    //    Property<"testOuter.testInner",int> innerInt;
    //    REQUIRE(a.getPropertyName()=="test");
    //    REQUIRE(a.getValue<int>() == 7);
    //    REQUIRE(simpleInt.getPropertyName()=="test5");
    //    REQUIRE(simpleInt.getValue()==5);
    //    REQUIRE(innerInt.getPropertyName()=="testOuter.testInner");
    //    REQUIRE(innerInt.getValue()==88);
}

TEST_CASE("Property context test") {
    auto contextPtr = bsc::Context::makeContext();

    REQUIRE(contextPtr->has<PropertyContext>() == true);
}