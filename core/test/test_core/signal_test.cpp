//
// Created by stilgar on 16.08.2019.
//

#include <catch2/catch.hpp>
#include <core/signal/Signal.h>
#include <core/signal/SimpleExecutor.h>

TEST_CASE("Signal test") {
    //before
    std::shared_ptr<Executor> executor = std::make_shared<SimpleExecutor>();
    //given
    int a = 5;
    int b = 7;
    Signal<int> signalI;
    signalI.assign([&](int a) { b = a; });
    REQUIRE(b == 7);
    //when
    signalI.signal(executor, a);
    //then
    REQUIRE(b == a);

}