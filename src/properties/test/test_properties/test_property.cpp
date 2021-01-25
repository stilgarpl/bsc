//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//
#include <catch2/catch.hpp>
#include <context/context/Context.h>
#include <list>
#include <properties/Property.h>
#include <properties/PropertyContext.h>
#include <properties/PropertyLoader.h>
using namespace bsc;

struct IProp {
    Property<"i", std::string> i{};
    int a = Property<"a">();
};

struct NestedProp : PropertyPrefix<"testOuter"> {
    int inner = Property<"testInner">();
};

TEST_CASE("Simple property test ") {
    auto context = Context::makeContext();
    Context::setActiveContext(context);

    PropertyLoader::parseToContext("test: 7\n"
                                   "test5: 5\n"
                                   "testOuter:\n"
                                   "  testInner: 88\n"
                                   "stringTest: string is with spaces\n"
                                   "sequence:\n"
                                   "  -  i: t1\n"
                                   "     a: 1\n"
                                   "  -  i: t2\n"
                                   "     a: 2\n"
                                   "  -  i: t3\n"
                                   "     a: 3\n");

    SECTION("Direct property") {
        Property<"test"> a;
        Property<"test"> b;
        int directIntA     = a;
        float directFloatA = a;
        REQUIRE(a.getPropertyName() == "test");
        REQUIRE(a.getValue<int>() == 7);
        REQUIRE(a.getPropertyName() == b.getPropertyName());
        REQUIRE(a.getValue<int>() == b.getValue<int>());
        REQUIRE(directIntA == 7);
        REQUIRE(directFloatA == 7.0f);
    }

    SECTION("Simple int property") {
        Property<"test5", int> simpleInt;
        REQUIRE(simpleInt.getPropertyName() == "test5");
        REQUIRE(simpleInt.getValue() == 5);
    }

    SECTION("Nested property") {
        Property<"testOuter.testInner", int> innerInt;
        REQUIRE(innerInt.getPropertyName() == "testOuter.testInner");
        REQUIRE(innerInt.getValue() == 88);
    }

    SECTION("String property") {
        Property<"stringTest", std::string> stringTest;
        REQUIRE(stringTest.getPropertyName() == "stringTest");
        REQUIRE(stringTest.getValue() == "string is with spaces");
    }

    SECTION("Container property") {
        Property<"sequence", std::list<IProp>> listTest;
        REQUIRE(listTest.getPropertyName() == "sequence");
        auto& list = listTest.getValue();
        REQUIRE(list.size() == 3);

        for (int count = 1; const auto& item : list) {
            REQUIRE(item.a == count);
            REQUIRE(item.i.getValue() == "t" + std::to_string(count));
            count++;
        }
    }

    SECTION("Nested property with prefix") {
        NestedProp nestedProp{};
        REQUIRE(nestedProp.inner == 88);
    }
}

TEST_CASE("Property context test") {
    auto contextPtr = bsc::Context::makeContext();

    REQUIRE(contextPtr->has<PropertyContext>() == true);
}