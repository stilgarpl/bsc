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


TEST_CASE("Property write test") {
    PropertyTextLoader loader("");
    class TestPropertyWriter : public PropertyWriter {
    private:
        PropertyIdSequence propertyId{};
        PropertyValueType propertyValue{};
    public:
        void resetNode() override {
        }
        void selectNode(const PropertyIdSequence& id) override {
            std::ranges::copy(id, std::back_inserter(propertyId));
        }
        void push() override {
        }
        void pop() override {
        }
        void setValue(const PropertyValueType& value) override {
            this->propertyValue += value;
            this->propertyValue += ":";
        }

        [[nodiscard]] const PropertyIdSequence& getPropertyId() const {
            return propertyId;
        }
        [[nodiscard]] const PropertyValueType& getPropertyValue() const {
            return propertyValue;
        }
        void nextEntry() override {
        }
        PropertyParserNodeType getNodeType() override {
            return PropertyParserNodeType::scalar;
        }
    };


    TestPropertyWriter writer;

    SECTION("int property") {
        Property<"property.int",int> intProperty(555);
        intProperty.save(writer);

        REQUIRE(writer.getPropertyId().size() == 2);
        REQUIRE(writer.getPropertyId()[0] == "property");
        REQUIRE(writer.getPropertyId()[1] == "int");
        REQUIRE(writer.getPropertyValue() == "555:");
    }
    SECTION("float property") {
        Property<"property.float",float> floatProperty(256.0);
        floatProperty.save(writer);

        REQUIRE(writer.getPropertyId().size() == 2);
        REQUIRE(writer.getPropertyId()[0] == "property");
        REQUIRE(writer.getPropertyId()[1] == "float");
        REQUIRE(writer.getPropertyValue() == "256.000000:");
    }
    SECTION("string property") {
        using namespace std::string_literals;
        Property<"property.string",std::string> stringProperty("test string"s);
        stringProperty.save(writer);

        REQUIRE(writer.getPropertyId().size() == 2);
        REQUIRE(writer.getPropertyId()[0] == "property");
        REQUIRE(writer.getPropertyId()[1] == "string");
        REQUIRE(writer.getPropertyValue() == "test string:");
    }
    SECTION("list property") {
        using namespace std::string_literals;
        std::list<Property<"a",std::string>> propertyList;
        propertyList.emplace_back("prop1"s);
        propertyList.emplace_back("prop2"s);
        propertyList.emplace_back("prop3"s);

        Property<"property.list",std::list<Property<"a",std::string>>> listProperty(propertyList);
        listProperty.save(writer);

        REQUIRE(writer.getPropertyId().size() == 5);
        REQUIRE(writer.getPropertyId()[0] == "property");
        REQUIRE(writer.getPropertyId()[1] == "list");
        REQUIRE(writer.getPropertyId()[2] == "a");
        REQUIRE(writer.getPropertyId()[3] == "a");
        REQUIRE(writer.getPropertyId()[4] == "a");
        REQUIRE(writer.getPropertyValue() == "prop1:prop2:prop3:");
    }

    SECTION("class property") {
        //@todo implement class property writing and test
    }
}

TEST_CASE("Assignable properties") {
    PropertyTextLoader loader("");

    Property<"property.path",int> intProperty {5};
    REQUIRE(intProperty.getValue() == 5);
    intProperty = 99;
    REQUIRE(intProperty.getValue() == 99);

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