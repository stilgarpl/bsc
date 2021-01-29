//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//
#include <catch2/catch.hpp>
#include <context/context/Context.h>
#include <list>
#include <properties/Property.h>
#include <properties/PropertyContext.h>
#include <properties/PropertyFileLoader.h>
#include <properties/PropertyTextLoader.h>
#include <testaid/testaid.h>
using namespace bsc;

struct IProp {
    Property<"i", std::string> i{};
    int a = Property<"a">();
};

struct NestedProp : PropertyPrefix<"testOuter"> {
    int inner = Property<"testInner">();
};

struct NestedProp2 {
    int inner = Property<"testInner2">();
};

struct FullProp {
    int test  = Property<"test">();
    int test5 = Property<"test5">();
    //    NestedProp testOuter1;
    Property<"testOuter2", NestedProp2> testOuter2;
    std::string stringTest = Property<"stringTest">();
    Property<"sequence", std::list<IProp>> sequence;
};

TEST_CASE("Property text test") {
    {
        PropertyTextLoader loader("test: 7\n"
                                  "test5: 5\n"
                                  "testOuter:\n"
                                  "  testInner: 88\n"
                                  "testOuter2:\n"
                                  "  testInner2: 99\n"
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

        SECTION("Default property") {
            int directDefaultProperty = Property<"key.not.present">(555);
            Property<"key.not.present", int> defaultProperty{789};
            REQUIRE(directDefaultProperty == 555);
            REQUIRE(defaultProperty() == 789);
        }

        SECTION("Property not present") {
            REQUIRE_THROWS_AS(Property<"key.not.present">().getValue<int>(), InvalidPropertyKeyException);
            using IntProperty = Property<"key.not.present", int>;
            REQUIRE_THROWS_AS(IntProperty{}, InvalidPropertyKeyException);
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

        SECTION("Full property mapping test") {
            FullProp fullProp{};
            REQUIRE(fullProp.test == 7);
            REQUIRE(fullProp.test5 == 5);
        }
    }

    REQUIRE_THROWS_AS(Property<"test">().getValue<int>(), PropertiesNotLoaded);
}

TEST_CASE("Property context test") {
    auto contextPtr = bsc::Context::makeContext();

    REQUIRE(contextPtr->has<PropertyContext>() == true);
}

TEST_CASE("Property file test") {

    struct Nested {
        int property     = Property<"property">();
        std::string text = Property<"text">();
    };

    struct Struct {
        std::string fieldA = Property<"fieldA">();
        Property<"fieldB", std::string> fieldB;
    };

    testaid::TestDirWithResources testDirWithResources;
    auto propertyFile = testDirWithResources.getResourcePath() / "application-properties.yaml";
    REQUIRE(fs::exists(propertyFile));

    PropertyFileLoader loader(propertyFile);
    int sampleProperty = Property<"sample-property">();
    REQUIRE(sampleProperty == 5);
    int defaultProperty = Property<"invalid.key">(8);
    REQUIRE(defaultProperty == 8);
    int nestedProperty = Property<"nested.property">();
    REQUIRE(nestedProperty == 7);
    Property<"nested.property", std::string> nestedString;
    REQUIRE(nestedString() == "7");
    Property<"nested.property", float> nestedFloat;
    REQUIRE(nestedFloat() == 7.0f);
    std::string nestedText = Property<"nested.text">();
    REQUIRE(nestedText == "text with spaces");

    Property<"nested", Nested> nested{};
    REQUIRE(nested().property == 7);
    REQUIRE(nested().text == "text with spaces");

    Nested nested2 = Property<"nested">();
    REQUIRE(nested2.property == 7);
    REQUIRE(nested2.text == "text with spaces");

    Property<"stringSequence", std::vector<std::string>> stringListProperty;
    REQUIRE(stringListProperty().size() == 3);
    REQUIRE(stringListProperty()[0] == "aaa");
    REQUIRE(stringListProperty()[1] == "bbb");
    REQUIRE(stringListProperty()[2] == "ccc");

    std::vector<std::string> stringList = Property<"stringSequence">();
    REQUIRE(stringList.size() == 3);
    REQUIRE(stringList[0] == "aaa");
    REQUIRE(stringList[1] == "bbb");
    REQUIRE(stringList[2] == "ccc");

    std::list<Struct> structSequence = Property<"structSequence">();
    REQUIRE(structSequence.size() == 3);
    for (int count = 1; const auto& item : structSequence) {
        REQUIRE(item.fieldA == "a" + std::to_string(count));
        REQUIRE(item.fieldB() == "b" + std::to_string(count));
        count++;
    }
}