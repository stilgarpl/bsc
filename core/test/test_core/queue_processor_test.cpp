//
// Created by stilgar on 14.10.2019.
//

#include <catch2/catch.hpp>

#include <thread>
#include <core/thread/ThreadQueueProcessor.h>
#include <functional>
#include <utility>

class TestProcessor : public ThreadQueueProcessor<int, int> {
//    std::function<void(int,int)> func ;
public:
    explicit TestProcessor(UpdateFuncType func) : ThreadQueueProcessor(std::move(func)) {}

};

TEST_CASE("Queue processor test") {
    Context::setActiveContext(Context::makeContext());
    int last_token = 0;
    int last_sender = 0;
    int TOKEN = 5;
    int VALUE = 17;
    {
        TestProcessor processor([&](int token, int& sender) {
            last_token = token;
            last_sender = sender;
        });
        processor.process(TOKEN, VALUE);
        std::this_thread::sleep_for(1ms);
    }
    REQUIRE(last_token == TOKEN);
    REQUIRE(last_sender == VALUE);


}