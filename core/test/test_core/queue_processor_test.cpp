//
// Created by stilgar on 14.10.2019.
//

#include <catch2/catch.hpp>

#include <thread>
#include <core/thread/ThreadQueueProcessor.h>
#include <functional>
#include <utility>


class TestProcessor : public bsc::ThreadQueueProcessor<int, int> {
//    std::function<void(int,int)> func ;
public:
    explicit TestProcessor(UpdateFuncType func) : ThreadQueueProcessor(std::move(func)) {}

};

TEST_CASE("Queue processor test") {
    bsc::Context::setActiveContext(bsc::Context::makeContext());
    int lastToken = 0;
    int lastSender = 0;
    int token = 5;
    int value = 17;
    {
        TestProcessor processor([&](int token, int& sender) {
            lastToken = token;
            lastSender = sender;
        });
        processor.process(token, value);
        std::this_thread::sleep_for(1ms);
    }
    REQUIRE(lastToken == token);
    REQUIRE(lastSender == value);


}