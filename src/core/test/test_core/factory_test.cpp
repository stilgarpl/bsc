//
// Created by Krzysztof Tulidowicz on 09.06.2020.
//
#include <catch2/catch.hpp>
#include <core/factory/CopyFactory.h>
#include <core/factory/FactoryPtr.h>
#include <core/factory/ParsingFactory.h>
#include <sstream>

using namespace bsc;

struct SampleTypeWithDefaultTraits {
    int a;
};

struct SampleTypeWithCustomSelector {
    int b;
};

struct SampleTypeWithCustomSelectorAndArgument {
    int c;
};
namespace bsc {
    template<>
    struct FactoryTraits<SampleTypeWithCustomSelector, NoFactorySpecialization> {
        using SelectorType = int;
        // using ArgumentType = ...
    };

    template<>
    struct FactoryTraits<SampleTypeWithCustomSelectorAndArgument, NoFactorySpecialization> {
        using SelectorType = int;
        using ArgumentType = std::vector<std::string>;
    };
}// namespace bsc

TEST_CASE("Copy factory test") {

    SECTION("SampleTypeWithDefaultTraits") {
        SampleTypeWithDefaultTraits sample{5};
        const std::string correctSelector   = "test";
        const std::string incorrectSelector = "incorrect";
        auto copyFactory                    = std::make_shared<CopyFactory<SampleTypeWithDefaultTraits>>();
        auto factoryPtr                     = copyFactory;
        copyFactory->addMold(correctSelector, sample);
        SECTION("correct selector") {
            auto producedObject = factoryPtr->create(correctSelector);
            REQUIRE(producedObject.a == sample.a);
        }
        SECTION("incorrect selector") { REQUIRE_THROWS_AS(factoryPtr->create(incorrectSelector), CopyFactoryNoMoldException); }
    }

    SECTION("SampleTypeWithCustomSelector") {
        SampleTypeWithCustomSelector sample{8};
        const int correctSelector   = 7;
        const int incorrectSelector = 8;
        auto copyFactory            = std::make_shared<CopyFactory<SampleTypeWithCustomSelector>>();
        auto factoryPtr             = copyFactory;
        copyFactory->addMold(correctSelector, sample);
        SECTION("correct selector") {
            auto producedObject = factoryPtr->create(correctSelector);
            REQUIRE(producedObject.b == sample.b);
        }
        SECTION("incorrect selector") { REQUIRE_THROWS_AS(factoryPtr->create(incorrectSelector), CopyFactoryNoMoldException); }
    }
}

TEST_CASE("Parsing factory test") {

    //    SECTION("SampleTypeWithDefaultTraits") {
    //        SampleTypeWithDefaultTraits sample{ 5};
    //        const std::string correctSelector = "test";
    //        const std::string incorrectSelector = "incorrect";
    //        auto parsingfactory = std::make_shared<Parsingfactory<SampleTypeWithDefaultTraits>>();
    //        FactoryPtr<SampleTypeWithDefaultTraits> factoryPtr = parsingfactory;
    //        parsingfactory->registerCreator(correctSelector,sample);
    //        SECTION("correct selector") {
    //            auto producedObject = factoryPtr->create(correctSelector);
    //            REQUIRE(producedObject.a == sample.a);
    //        }
    //        SECTION("incorrect selector") {
    //            REQUIRE_THROWS_AS(factoryPtr->create(incorrectSelector),FactoryInvalidSelector);
    //        }
    //
    //    }

    SECTION("SampleTypeWithCustomSelectorAndArgument with sample") {
        SampleTypeWithCustomSelectorAndArgument sample{8};
        const int correctSelector         = 7;
        const int correctSelectorArgument = 2;
        const int incorrectSelector       = 8;
        auto parsingFactory               = std::make_shared<ParsingFactory<SampleTypeWithCustomSelectorAndArgument>>();
        auto factoryPtr                   = parsingFactory;
        parsingFactory->registerCreator(correctSelector, sample);
        SECTION("correct selector") {
            auto producedObject = factoryPtr->create(correctSelector, {std::to_string(correctSelectorArgument)});
            REQUIRE(producedObject.c == sample.c);
        }
        SECTION("incorrect selector") { REQUIRE_THROWS_AS(factoryPtr->create(incorrectSelector, {}), FactoryInvalidSelector); }
    }

    SECTION("SampleTypeWithCustomSelectorAndArgument with simple creator") {
        const int correctSelector         = 7;
        const int correctSelectorArgument = 2;
        const int incorrectSelector       = 8;
        auto parsingFactory               = std::make_shared<ParsingFactory<SampleTypeWithCustomSelectorAndArgument>>();
        auto factoryPtr                   = parsingFactory;
        parsingFactory->registerCreator(correctSelector, [](auto a) { return SampleTypeWithCustomSelectorAndArgument{9}; });
        SECTION("correct selector") {
            auto producedObject = factoryPtr->create(correctSelector, {std::to_string(correctSelectorArgument)});
            REQUIRE(producedObject.c == 9);
        }
        SECTION("incorrect selector") { REQUIRE_THROWS_AS(factoryPtr->create(incorrectSelector, {}), FactoryInvalidSelector); }
    }

    SECTION("SampleTypeWithCustomSelectorAndArgument with magic creator") {
        const int correctSelector         = 7;
        const int correctSelectorArgument = 2;
        auto creator                      = [](int x) { return SampleTypeWithCustomSelectorAndArgument{x}; };
        const int incorrectSelector       = 8;
        auto parsingFactory               = std::make_shared<ParsingFactory<SampleTypeWithCustomSelectorAndArgument>>();
        auto factoryPtr                   = parsingFactory;
        parsingFactory->registerCreator(correctSelector, *creator);
        SECTION("correct selector") {
            auto producedObject = factoryPtr->create(correctSelector, {std::to_string(correctSelectorArgument)});
            REQUIRE(producedObject.c == correctSelectorArgument);
        }
        SECTION("incorrect selector") { REQUIRE_THROWS_AS(factoryPtr->create(incorrectSelector, {}), FactoryInvalidSelector); }
    }
}

TEST_CASE("Autoregistering factories") {
    class Aa {};
    // registered factory will be CopyFactory<Aa>, not AaFactory, because that's what the parameter is CopyFactory: AutoFactory - if it's a
    // problem, add parameter to Copy/Parsing factories.
    class AaFactory : public CopyFactory<Aa> {};

    Context::setActiveContext(Context::makeContext());

    auto factoryContext = Context::getActiveContext()->get<FactoryContext>();
    REQUIRE(factoryContext->hasFactory<Aa>() == true);
    FactoryPtr<Aa> sampleFactory;
    REQUIRE(sampleFactory.isValid() == true);
}