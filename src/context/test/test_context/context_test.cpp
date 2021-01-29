//
// Created by Krzysztof Tulidowicz on 16.08.2019.
//

#include <catch2/catch.hpp>
#include <context/context/Context.h>
#include <thread>

#include <sstream>

using namespace bsc;

TEST_CASE("Context test") {
    const std::string contextName = "context field name";
    const int value               = 5;

    Context::OwnPtr localContext = Context::makeContext();
    SECTION("active context test") {
        REQUIRE(!Context::hasActiveContext());
        Context::setActiveContext(localContext);
        REQUIRE(Context::getActiveContext() == localContext);

        SECTION("active context thread test") {
            REQUIRE(Context::getActiveContext() == localContext);
            Context::OwnPtr otherContext = Context::makeContext(localContext);
            // each thread has its own context, so new thread won't have an active context until setDirect.
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
        REQUIRE(*localContext->get<int>(contextName) == value);
        REQUIRE(*childContext->get<int>(contextName) == value);
    }

    SECTION("child context value") {
        REQUIRE(!localContext->has<int>(contextName));
        REQUIRE(!childContext->has<int>(contextName));
        childContext->setKey<int>(contextName, value);
        REQUIRE(!localContext->has<int>(contextName));
        REQUIRE(childContext->has<int>(contextName));
        REQUIRE(*childContext->get<int>(contextName) == value);
    }
}

TEST_CASE("Context initializer test") {
    Context::registerInitializer([](Context& context) { context.set<int>(5); });

    Context::OwnPtr context = Context::makeContext();
    REQUIRE(context->has<int>());
    REQUIRE(*context->get<int>() == 5);
}

TEST_CASE("Context loop test") {
    Context::OwnPtr context1 = Context::makeContext();
    Context::OwnPtr context2 = Context::makeContext();
    context2->setParentContext(context1);
    Context::OwnPtr context3 = Context::makeContext();
    context3->setParentContext(context2);
    REQUIRE_THROWS_AS(context1->setParentContext(context3), bsc::ContextLoopException);
}

TEST_CASE("SetLocalContext test") {
    Context::OwnPtr context1 = Context::makeContext();
    Context::OwnPtr context2 = Context::makeContext();
    Context::setActiveContext(context1);
    SECTION("SetLocalContext should set context2 as active context and restore context1 after leaving the scope") {
        REQUIRE(Context::getActiveContext() == context1);
        {
            SetLocalContext l{context2};
            REQUIRE(Context::getActiveContext() == context2);
        }
        REQUIRE(Context::getActiveContext() == context1);
    }
    SECTION("SetLocalContext should run callback in local context") {
        REQUIRE(Context::getActiveContext() == context1);
        SetLocalContext{context2}([&]() { REQUIRE(Context::getActiveContext() == context2); });
        REQUIRE(Context::getActiveContext() == context1);
    }
    SECTION("SetLocalContext should run callback in local context - multi callback") {
        REQUIRE(Context::getActiveContext() == context1);
        SetLocalContext{context2}([&]() { REQUIRE(Context::getActiveContext() == context2); })(
                [&]() { REQUIRE(Context::getActiveContext() == context2); })([&]() { REQUIRE(Context::getActiveContext() == context2); });
        REQUIRE(Context::getActiveContext() == context1);
    }

    SECTION("SetLocalContext should run callback in local context - with parameters") {
        const int argument = 5;
        REQUIRE(Context::getActiveContext() == context1);
        SetLocalContext{context2}(
                [&](auto i) {
                    REQUIRE(i == argument);
                    REQUIRE(Context::getActiveContext() == context2);
                },
                argument);
        REQUIRE(Context::getActiveContext() == context1);
    }
}

TEST_CASE("ContextRunner test") {
    Context::OwnPtr context1 = Context::makeContext();
    Context::OwnPtr context2 = Context::makeContext();
    Context::setActiveContext(context1);
    const auto threadId = std::this_thread::get_id();

    SECTION("ContextRunner::run test") {
        ContextRunner::run([&]() {
            REQUIRE(Context::getActiveContext() == context1);
            REQUIRE(threadId == std::this_thread::get_id());
        });
        ContextRunner::run(context2, [&]() {
            REQUIRE(Context::getActiveContext() == context2);
            REQUIRE(threadId == std::this_thread::get_id());
        });
        const int argument = 5;
        ContextRunner::run(
                context2,
                [&](auto i) {
                    REQUIRE(i == argument);
                    REQUIRE(Context::getActiveContext() == context2);
                    REQUIRE(threadId == std::this_thread::get_id());
                },
                argument);
    }
    SECTION("ContextRunner::runNewThread test") {
        auto t1            = ContextRunner::runNewThread([&]() {
            REQUIRE(Context::getActiveContext() == context1);
            REQUIRE(threadId != std::this_thread::get_id());
        });
        auto t2            = ContextRunner::runNewThread(context2, [&]() {
            REQUIRE(Context::getActiveContext() == context2);
            REQUIRE(threadId != std::this_thread::get_id());
        });
        const int argument = 5;
        auto t3            = ContextRunner::runNewThread(
                context2,
                [&](auto i) {
                    REQUIRE(i == argument);
                    REQUIRE(Context::getActiveContext() == context2);
                    REQUIRE(threadId != std::this_thread::get_id());
                },
                argument);
    }
}