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

    Context::OwnPtr localContext = Context::makeContext();
    SECTION("active context test") {
        REQUIRE(!Context::hasActiveContext());
        Context::setActiveContext(localContext);
        REQUIRE(Context::getActiveContext() == localContext);

        SECTION("active context thread test") {
            REQUIRE(Context::getActiveContext() == localContext);
            Context::OwnPtr otherContext = Context::makeContext(localContext);
            //each thread has its own context, so new thread won't have an active context until setDirect.
            std::thread([&otherContext]() {
                REQUIRE(!Context::hasActiveContext());
                Context::setActiveContext(otherContext);
                REQUIRE(Context::getActiveContext() == otherContext);
            }).join();
            REQUIRE(Context::getActiveContext() == localContext);
        }

    }
    Context::OwnPtr childContext = Context::makeContext(localContext);

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


class TestInputOutputContext : public InputOutputContext {
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
    Context::OwnPtr context = Context::makeContext();
    Context::setActiveContext(context);
    context->setDirect<InputOutputContext>(std::make_shared<TestInputOutputContext>());

    auto& out = context->get<InputOutputContext>().out();
    auto& in = context->get<InputOutputContext>().in();
    auto& stream = static_cast<TestInputOutputContext&>(context->get<InputOutputContext>()).getStream();

    const std::string testString = "TEST STRING";
    out << testString;
    REQUIRE(stream.str() == testString);
    std::string result;
    in >> result;
    REQUIRE(result == "TEST"); //first word

}

TEST_CASE("Invalid context test") {
    Context::OwnPtr context = Context::makeContext();
    REQUIRE_THROWS_AS(context->setDirect<InputOutputContext>(std::make_shared<int>()), InvalidContextException);
}

TEST_CASE("Context loop test") {
    Context::OwnPtr context1 = Context::makeContext();
    Context::OwnPtr context2 = Context::makeContext();
    context2->setParentContext(context1);
    Context::OwnPtr context3 = Context::makeContext();
    context3->setParentContext(context2);
    REQUIRE_THROWS_AS(context1->setParentContext(context3), ContextLoopException);
}