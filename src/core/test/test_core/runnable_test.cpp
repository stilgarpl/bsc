//
// Created by stilgar on 08.10.2020.
//

#include <catch2/catch.hpp>
#include <core/thread/Runnable.h>
#include <testaid/testaid.h>
#include <utility>

using namespace bsc;
using namespace std::chrono_literals;

class TestRunnable : public Runnable {
    std::function<void(void)> f;

public:
    explicit TestRunnable(std::function<void(void)> f) : f(std::move(f)) {}
    void run() override {
        while (!isStopping()) {
            f();
        }
    }
};

TEST_CASE("Runnable test") {
    Context::OwnPtr context = Context::makeContext();
    Context::setActiveContext(context);
    std::atomic_int count = 0;
    TestRunnable runnable([&count]() { count++; });
    runnable.start();
    testaid::waitFor([&count]() { return count > 0; }, 1000ms);
    REQUIRE(runnable.isStarted());
    REQUIRE(!runnable.isStopping());
    std::this_thread::sleep_for(10ms);
    runnable.stop();
    REQUIRE(runnable.isStopping());
    runnable.join();
    REQUIRE(runnable.isFinished());
    REQUIRE(count > 0);
}