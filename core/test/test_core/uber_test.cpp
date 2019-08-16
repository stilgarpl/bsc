//
// Created by stilgar on 16.08.2019.
//

#include <core/uber/Uber.h>
#include <list>
#include <catch2/catch.hpp>


TEST_CASE("Uber test") {
    const int MAX_TEST_INT = 51;
    const int MAX_TEST_FLOAT = 722;
    const int MAX_TEST_STRING = 39;
    Uber<std::list> uberList;
    for (uint i = 0; i < MAX_TEST_INT; i++) {
        uberList.get<int>().push_back(i);
    }
    for (uint i = 0; i < MAX_TEST_FLOAT; i++) {
        uberList.get<float>().push_back(0.5f * i);
    }
    for (uint i = 0; i < MAX_TEST_STRING; i++) {
        uberList.get<std::string>().push_back(std::to_string(i));
    }
    {
        auto it = uberList.get<int>().begin();
        for (auto i = 0; i < MAX_TEST_INT; i++, it++) {
            REQUIRE(*it == i);
        }
    }
    {
        auto it = uberList.get<float>().begin();
        for (uint i = 0; i < MAX_TEST_FLOAT; i++, it++) {
            REQUIRE(*it == 0.5f * i);
        }
    }
    {
        auto it = uberList.get<std::string>().begin();
        for (uint i = 0; i < MAX_TEST_STRING; i++, it++) {
            auto i_val = std::to_string(i);
            auto it_val = *it;
            REQUIRE(i_val == it_val);
        }
    }


}