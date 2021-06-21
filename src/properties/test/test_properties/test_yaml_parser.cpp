//
// Created by Krzysztof Tulidowicz on 21.01.2021.
//
#include <catch2/catch.hpp>
#include <context/context/Context.h>
#include <properties/Property.h>
#include <properties/PropertyContext.h>
#include <properties/control/parser/YamlParser.h>
using namespace bsc;

TEST_CASE("YamlParser test ") {

    std::string yaml = "invoice: 34843\n"
                       "date   : !!str 2001-01-23\n"
                       "bill-to: &id001\n"
                       "    given  : Chris\n"
                       "    family : Dumars\n"
                       "    address:\n"
                       "        lines: |\n"
                       "            458 Walkman Dr.\n"
                       "            Suite #292\n"
                       "sequence:\n"
                       "  -  i: t1\n"
                       "  -  i: t2\n"
                              "  -  i: t3\n"
                              "outerSequence:\n"
                              "  - innerSequence:\n"
                              "      - i: a1\n"
                              "      - i: a2\n"
                              "      - i: a3\n"
                              "  - innerSequence:\n"
                              "      - i: b1\n"
                              "      - i: b2\n"
                              "      - i: b3\n";

    YamlParser yamlParser{yaml};

    SECTION("selectNode test ") {
        auto rootType = yamlParser.getNodeType();
        REQUIRE(rootType == PropertyNodeType::map);
        yamlParser.selectNode({{"invoice"}});
        auto type  = yamlParser.getNodeType();
        auto value = yamlParser.getValue();
        REQUIRE(type == PropertyNodeType::scalar);
        REQUIRE(value == "34843");
        //        yamlParser.selectNode({"date"});
        //        REQUIRE(yamlParser.getNodeType() == PropertyNodeType::invalid);
        yamlParser.resetNode();
        yamlParser.selectNode({"date"});
        REQUIRE(yamlParser.getNodeType() == PropertyNodeType::scalar);
        REQUIRE(yamlParser.getValue() == "2001-01-23");
        yamlParser.resetNode();
        yamlParser.selectNode({"bill-to"});
        REQUIRE(yamlParser.getNodeType() == PropertyNodeType::map);
        REQUIRE(yamlParser.bsc::PropertyParser::getValue({{"given"}}) == "Chris");
        REQUIRE(yamlParser.bsc::PropertyParser::getValue({{"family"}}) == "Dumars");
        REQUIRE(yamlParser.bsc::PropertyParser::getValue({{"address"}, {"lines"}}) == "458 Walkman Dr.\nSuite #292\n");
    }

    SECTION("sequence test") {
        auto rootType = yamlParser.getNodeType();
        REQUIRE(rootType == PropertyNodeType::map);
        yamlParser.selectNode({{"sequence"}});
        REQUIRE(yamlParser.getNodeType() == PropertyNodeType::sequence);
        REQUIRE(yamlParser.bsc::PropertyParser::getValue({{"i"}}) == "t1");
        yamlParser.nextEntry();
        REQUIRE(yamlParser.bsc::PropertyParser::getValue({{"i"}}) == "t2");
        yamlParser.nextEntry();
        REQUIRE(yamlParser.bsc::PropertyParser::getValue({{"i"}}) == "t3");
        yamlParser.nextEntry();
    }

    SECTION("nested sequence test") {
        auto rootType = yamlParser.getNodeType();
        REQUIRE(rootType == PropertyNodeType::map);
        yamlParser.selectNode({{"outerSequence"}});
        REQUIRE(yamlParser.getNodeType() == PropertyNodeType::sequence);
        yamlParser.push();
        yamlParser.selectNode({{"innerSequence"}});
        REQUIRE(yamlParser.getNodeType() == PropertyNodeType::sequence);
        REQUIRE(yamlParser.hasEntry() == true);
        REQUIRE(yamlParser.bsc::PropertyParser::getValue({{"i"}}) == "a1");
        yamlParser.nextEntry();
        REQUIRE(yamlParser.hasEntry() == true);
        REQUIRE(yamlParser.bsc::PropertyParser::getValue({{"i"}}) == "a2");
        yamlParser.nextEntry();
        REQUIRE(yamlParser.hasEntry() == true);
        REQUIRE(yamlParser.bsc::PropertyParser::getValue({{"i"}}) == "a3");
        yamlParser.nextEntry();
        REQUIRE(yamlParser.hasEntry() == false);
        yamlParser.pop();
        yamlParser.nextEntry();
        yamlParser.selectNode({{"innerSequence"}});
        REQUIRE(yamlParser.hasEntry() == true);
        REQUIRE(yamlParser.bsc::PropertyParser::getValue({{"i"}}) == "b1");
        yamlParser.nextEntry();
        REQUIRE(yamlParser.hasEntry() == true);
        REQUIRE(yamlParser.bsc::PropertyParser::getValue({{"i"}}) == "b2");
        yamlParser.nextEntry();
        REQUIRE(yamlParser.hasEntry() == true);
        REQUIRE(yamlParser.bsc::PropertyParser::getValue({{"i"}}) == "b3");
        yamlParser.nextEntry();
        REQUIRE(yamlParser.hasEntry() == false);
    }
}