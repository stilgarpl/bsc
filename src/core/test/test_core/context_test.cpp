//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//
#include <catch2/catch.hpp>
#include <context/context/Context.h>
#include <core/io/InputOutputContext.h>
#include <thread>

#include <sstream>

using namespace bsc;

class TestInputOutputContext : public bsc::InputOutputContext {
    std::stringstream stream;
    std::stringstream errorStream;

public:
    std::ostream& out() override { return stream; }

    std::ostream& err() override { return errorStream; }

    std::istream& in() override { return stream; }

    const auto& getStream() const { return stream; }

    const auto& getErrorStream() const { return errorStream; }
};

TEST_CASE("InputOutput Context test") {
    Context::OwnPtr context = Context::makeContext();
    Context::setActiveContext(context);
    SECTION("TestInputOutputContext") {
        auto ctx = std::make_shared<TestInputOutputContext>();
        context->setDirect<InputOutputContext>(ctx);

        REQUIRE(&InputOutputContext::active() == &*ctx);

        auto& out         = context->get<InputOutputContext>()->out();
        auto& in          = context->get<InputOutputContext>()->in();
        auto& err         = context->get<InputOutputContext>()->err();
        auto& stream      = ctx->getStream();
        auto& errorStream = ctx->getErrorStream();

        const std::string testString  = "TEST STRING";
        const std::string errorString = "ERROR STRING";
        out << testString;
        REQUIRE(stream.str() == testString);
        std::string result;
        in >> result;
        REQUIRE(result == "TEST");// first word
        in >> result;
        REQUIRE(result == "STRING");// second word
        err << errorString;
        REQUIRE(errorStream.str() == errorString);
    }

    SECTION("StandardInputOutputContext") {
        auto ctx = std::make_shared<StandardInputOutputContext>();
        context->setDirect<InputOutputContext>(ctx);

        REQUIRE(&InputOutputContext::active() == &*ctx);

        auto& out = context->get<InputOutputContext>()->out();
        auto& in  = context->get<InputOutputContext>()->in();
        auto& err = context->get<InputOutputContext>()->err();
        std::stringstream stream;
        std::stringstream errorStream;
        // Backup streambuffers
        auto streamBufferCout = std::cout.rdbuf();
        auto streamBufferCin  = std::cin.rdbuf();
        auto streamBufferErr  = std::cerr.rdbuf();

        // Redirect cout to file
        std::cout.rdbuf(stream.rdbuf());
        std::cin.rdbuf(stream.rdbuf());
        std::cerr.rdbuf(errorStream.rdbuf());

        const std::string testString  = "TEST STRING";
        const std::string errorString = "ERROR STRING";

        out << testString;
        REQUIRE(stream.str() == testString);
        std::string result;
        in >> result;
        REQUIRE(result == "TEST");// first word
        in >> result;
        REQUIRE(result == "STRING");// second word
        err << errorString;
        REQUIRE(errorStream.str() == errorString);
        //@todo use RAII to restore this: - and also use a class to redirect those in testaid
        std::cout.rdbuf(streamBufferCout);
        std::cin.rdbuf(streamBufferCin);
        std::cerr.rdbuf(streamBufferErr);
    }
}

TEST_CASE("Invalid context test") {
    Context::OwnPtr context = Context::makeContext();
    REQUIRE_THROWS_AS(context->setDirect<bsc::InputOutputContext>(std::make_shared<int>()), bsc::InvalidContextException);
}