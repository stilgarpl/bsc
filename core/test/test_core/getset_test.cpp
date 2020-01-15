//
// Created by stilgar on 12.12.2019.
//

#include <catch2/catch.hpp>
#include <core/getset/GetSet.h>

using namespace bsc;
class TestClass {
public:
    int _x, _y;

    auto x() {
        return bsc::GetSet(_x);
    }

    auto y() {
        return bsc::GetSet(_y);
    }
};


TEST_CASE("GetSet test") {

    TestClass t;
    const int x_val = 5;
    const int y_val = 6;

    t.x() = x_val;
    t.y() = y_val;
    REQUIRE(t._x == x_val);
    REQUIRE(t._y == y_val);
    REQUIRE(t.x() == x_val);
    REQUIRE(t.y() == y_val);


}