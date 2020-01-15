//
// Created by stilgar on 16.08.2019.
//

#include <core/uber/Uber.h>
#include <list>
#include <catch2/catch.hpp>



TEST_CASE("Uber test") {
    const int maxTestInt = 51;
    const int maxTestFloat = 722;
    const int maxTestString = 39;
    bsc::Uber<std::list> uberList;
    for (unsigned i = 0; i < maxTestInt; i++) {
        uberList.get<int>().push_back(i);
    }
    for (unsigned i = 0; i < maxTestFloat; i++) {
        uberList.get<float>().push_back(0.5f * i);
    }
    for (unsigned i = 0; i < maxTestString; i++) {
        uberList.get<std::string>().push_back(std::to_string(i));
    }
    {
        auto it = uberList.get<int>().begin();
        for (auto i = 0; i < maxTestInt; i++, it++) {
            REQUIRE(*it == i);
        }
    }
    {
        auto it = uberList.get<float>().begin();
        for (unsigned i = 0; i < maxTestFloat; i++, it++) {
            REQUIRE(*it == 0.5f * i);
        }
    }
    {
        auto it = uberList.get<std::string>().begin();
        for (unsigned i = 0; i < maxTestString; i++, it++) {
            auto iVal = std::to_string(i);
            auto itVal = *it;
            REQUIRE(iVal == itVal);
        }
    }


}