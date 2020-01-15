//
// Created by stilgar on 16.08.2019.
//

#include <catch2/catch.hpp>
#include <core/context/Context.h>
#include <thread>
#include <core/io/InputOutputContext.h>
#include <sstream>


TEST_CASE("Context test") {
    const std::string contextName = "context field name";
    const int value = 5;

    bsc::Context::OwnPtr localContext = bsc::Context::makeContext();
    SECTION("active context test") {
        REQUIRE(!bsc::Context::hasActiveContext());
        bsc::Context::setActiveContext(localContext);
        REQUIRE(bsc::Context::getActiveContext() == localContext);

        SECTION("active context thread test") {
            REQUIRE(bsc::Context::getActiveContext() == localContext);
            bsc::Context::OwnPtr otherContext = bsc::Context::makeContext(localContext);
            //each thread has its own context, so new thread won't have an active context until setDirect.
            std::thread([&otherContext]() {
                REQUIRE(!bsc::Context::hasActiveContext());
                bsc::Context::setActiveContext(otherContext);
                REQUIRE(bsc::Context::getActiveContext() == otherContext);
            }).join();
            REQUIRE(bsc::Context::getActiveContext() == localContext);
        }

    }
    bsc::Context::OwnPtr childContext = bsc::Context::makeContext(localContext);

    SECTION("parent context value") {
        REQUIRE(!localContext->has<int>(contextName));
        REQUIRE(!childContext->has<int>(contextName));
        localContext->setKey<int>(contextName, value);
        REQUIRE(localContext->has<int>(contextName));
        REQUIRE(childContext->has<int>(contextName));
        REQUIRE(localContext->get<int>(contextName) == value);
        REQUIRE(childContext->get<int>(contextName) == value);
    }

    SECTION("child context value") {
        REQUIRE(!localContext->has<int>(contextName));
        REQUIRE(!childContext->has<int>(contextName));
        childContext->setKey<int>(contextName, value);
        REQUIRE(!localContext->has<int>(contextName));
        REQUIRE(childContext->has<int>(contextName));
        REQUIRE(childContext->get<int>(contextName) == value);
    }


}


class TestInputOutputContext : public bsc::InputOutputContext {
    std::stringstream stream;
public:
    std::ostream& out() override {
        return stream;
    }

    std::ostream& err() override {
        return stream;
    }

    std::istream& in() override {
        return stream;
    }

    const std::stringstream& getStream() const {
        return stream;
    }
};

TEST_CASE("InputOutput Context test") {
    bsc::Context::OwnPtr context = bsc::Context::makeContext();
    bsc::Context::setActiveContext(context);
    context->setDirect<bsc::InputOutputContext>(std::make_shared<TestInputOutputContext>());

    auto& out = context->get<bsc::InputOutputContext>().out();
    auto& in = context->get<bsc::InputOutputContext>().in();
    auto& stream = static_cast<TestInputOutputContext&>(context->get<bsc::InputOutputContext>()).getStream();

    const std::string testString = "TEST STRING";
    out << testString;
    REQUIRE(stream.str() == testString);
    std::string result;
    in >> result;
    REQUIRE(result == "TEST"); //first word

}

TEST_CASE("Invalid context test") {
    bsc::Context::OwnPtr context = bsc::Context::makeContext();
    REQUIRE_THROWS_AS(context->setDirect<bsc::InputOutputContext>(std::make_shared<int>()),
                      bsc::InvalidContextException);
}

TEST_CASE("Context loop test") {
    bsc::Context::OwnPtr context1 = bsc::Context::makeContext();
    bsc::Context::OwnPtr context2 = bsc::Context::makeContext();
    context2->setParentContext(context1);
    bsc::Context::OwnPtr context3 = bsc::Context::makeContext();
    context3->setParentContext(context2);
    REQUIRE_THROWS_AS(context1->setParentContext(context3), bsc::ContextLoopException);
}