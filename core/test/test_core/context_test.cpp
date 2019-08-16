//
// Created by stilgar on 16.08.2019.
//

#include <catch2/catch.hpp>
#include <core/context/Context.h>
#include <thread>

TEST_CASE("Context test") {
    const std::string CONTEXT_NAME = "context field name";
    const int VALUE = 5;

    Context::OwnPtr localContext = Context::makeContext();
    SECTION("active context test") {
        REQUIRE(Context::getActiveContext() == nullptr);
        Context::setActiveContext(localContext);
        REQUIRE(Context::getActiveContext() == localContext);

        SECTION("active context thread test") {
            REQUIRE(Context::getActiveContext() == localContext);
            Context::OwnPtr otherContext = Context::makeContext(localContext);
            //each thread has its own context, so new thread won't have an active context until set.
            std::thread([&otherContext]() {
                REQUIRE(Context::getActiveContext() == nullptr);
                Context::setActiveContext(otherContext);
                REQUIRE(Context::getActiveContext() == otherContext);
            }).join();
            REQUIRE(Context::getActiveContext() == localContext);
        }

    }
    Context::OwnPtr childContext = Context::makeContext(localContext);

    SECTION("parent context value") {
        REQUIRE(localContext->get<int>(CONTEXT_NAME) == nullptr);
        REQUIRE(childContext->get<int>(CONTEXT_NAME) == nullptr);
        localContext->setKey<int>(CONTEXT_NAME, VALUE);
        REQUIRE(localContext->get<int>(CONTEXT_NAME) != nullptr);
        REQUIRE(childContext->get<int>(CONTEXT_NAME) != nullptr);
        REQUIRE(*localContext->get<int>(CONTEXT_NAME) == VALUE);
        REQUIRE(*childContext->get<int>(CONTEXT_NAME) == VALUE);
    }

    SECTION("child context value") {
        REQUIRE(localContext->get<int>(CONTEXT_NAME) == nullptr);
        REQUIRE(childContext->get<int>(CONTEXT_NAME) == nullptr);
        childContext->setKey<int>(CONTEXT_NAME, VALUE);
        REQUIRE(localContext->get<int>(CONTEXT_NAME) == nullptr);
        REQUIRE(childContext->get<int>(CONTEXT_NAME) != nullptr);
        REQUIRE(*childContext->get<int>(CONTEXT_NAME) == VALUE);
    }


}