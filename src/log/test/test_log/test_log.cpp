//
// Created by Krzysztof Tulidowicz on 11.02.2021.
//

#include <catch2/catch.hpp>
#include <context/context/Context.h>
#include <log/log/Logger.h>
#include <log/log/LoggerContext.h>
using namespace bsc;

TEST_CASE("Logger context test") {
    auto contextPtr = bsc::Context::makeContext(true);

    REQUIRE(contextPtr->has<LoggerContext>() == true);
}
