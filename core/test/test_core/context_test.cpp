//
// Created by stilgar on 16.08.2019.
//

#include <catch2/catch.hpp>
#include <core/context/Context.h>
#include <thread>
#include <core/io/InputOutputContext.h>

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
            //each thread has its own context, so new thread won't have an active context until set.
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
        REQUIRE(localContext->get<int>(contextName) == nullptr);
        REQUIRE(childContext->get<int>(contextName) == nullptr);
        localContext->setKey<int>(contextName, value);
        REQUIRE(localContext->get<int>(contextName) != nullptr);
        REQUIRE(childContext->get<int>(contextName) != nullptr);
        REQUIRE(*localContext->get<int>(contextName) == value);
        REQUIRE(*childContext->get<int>(contextName) == value);
    }

    SECTION("child context value") {
        REQUIRE(localContext->get<int>(contextName) == nullptr);
        REQUIRE(childContext->get<int>(contextName) == nullptr);
        childContext->setKey<int>(contextName, value);
        REQUIRE(localContext->get<int>(contextName) == nullptr);
        REQUIRE(childContext->get<int>(contextName) != nullptr);
        REQUIRE(*childContext->get<int>(contextName) == value);
    }


}


class TestInputOutputContext : public InputOutputContext {
    std::stringstream stream;
public:
    std::ostream& out() override {
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
    context->setContext<InputOutputContext, TestInputOutputContext>();

    auto& out = context->get<InputOutputContext>()->out();
    auto& in = context->get<InputOutputContext>()->in();
    auto& stream = std::static_pointer_cast<TestInputOutputContext>(context->get<InputOutputContext>())->getStream();

    const std::string testString = "TEST STRING";
    out << testString;
    REQUIRE(stream.str() == testString);
    std::string result;
    in >> result;
    REQUIRE(result == "TEST"); //first word

}