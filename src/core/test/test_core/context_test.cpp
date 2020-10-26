//
// Created by Krzysztof Tulidowicz on 16.08.2019.
//

#include <catch2/catch.hpp>
#include <core/context/Context.h>
#include <thread>
#include <core/io/InputOutputContext.h>
#include <sstream>

using namespace bsc;

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
    REQUIRE_THROWS_AS(context->setDirect<bsc::InputOutputContext>(std::make_shared<int>()),
                      bsc::InvalidContextException);
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