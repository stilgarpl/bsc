//
// Created by Krzysztof Tulidowicz on 06.01.2021.
//

#include <catch2/catch.hpp>
#include <logic/state/StateMachine.h>
#include <testaid/testaid.h>

using namespace bsc;

class TestStateMachine : public StateMachine<int> {

    static DefinitionPtr makeDefinition() {
        auto ptr = std::make_shared<StateMachineDefinition<int>>();
        ptr->addState(1, 2, 3, 4, 5, 6);
        ptr->addLink(1, 2, 3, 4, 5);
        ptr->addLink(2, 3, 4, 5, 6);
        ptr->addLink(3, 1);
        ptr->addLink(4, 1);
        ptr->addLink(5, 1);
        ptr->addLink(6, 1);
        return ptr;
    }

public:
    TestStateMachine(const std::function<void(const int&)>& onEnterStateHandler,
                     const std::function<void(const int&)>& onLeaveStateHandler,
                     const std::function<void(const int&)>& invalidStateHandler,
                     const std::function<void(const int&)>& invalidChangeHandler)
        : StateMachine(makeDefinition(), onEnterStateHandler, onLeaveStateHandler, invalidStateHandler, invalidChangeHandler) {}
};

TEST_CASE("State machine test") {
    std::optional<int> invalidState;
    std::optional<int> invalidChange;
    std::optional<int> enteredState;
    std::optional<int> leftState;
    auto invalidStateHandler  = [&](const int& state) { invalidState = state; };
    auto invalidChangeHandler = [&](const int& state) { invalidChange = state; };
    auto enteredStateHandler  = [&](const int& state) { enteredState = state; };
    auto leftStateHandler     = [&](const int& state) { leftState = state; };

    TestStateMachine testStateMachine(enteredStateHandler, leftStateHandler, invalidStateHandler, invalidChangeHandler);
    REQUIRE(enteredState.has_value() == false);
    REQUIRE(leftState.has_value() == false);
    REQUIRE(invalidState.has_value() == false);
    REQUIRE(invalidChange.has_value() == false);
    SECTION("valid state transition") {
        // given
        testStateMachine.setState(1);

        // when
        testStateMachine.changeState(2);
        // then
        REQUIRE(invalidState.has_value() == false);
        REQUIRE(invalidChange.has_value() == false);
        REQUIRE(enteredState.has_value() == true);
        REQUIRE(leftState.has_value() == true);
        REQUIRE(*enteredState == 2);
        REQUIRE(*leftState == 1);
    }

    SECTION("uninitialized state machine") {
        // given
        // when
        REQUIRE_THROWS_AS(testStateMachine.changeState(2), TestStateMachine::StateMachineUninitialized);
        // then
        REQUIRE(invalidState.has_value() == false);
        REQUIRE(invalidChange.has_value() == false);
        REQUIRE(enteredState.has_value() == false);
        REQUIRE(leftState.has_value() == false);
    }

    SECTION("invalid transition") {
        // given
        testStateMachine.setState(1);
        // when
        testStateMachine.changeState(6);
        // then
        REQUIRE(invalidState.has_value() == false);
        REQUIRE(invalidChange.has_value() == true);
        REQUIRE(enteredState.has_value() == false);
        REQUIRE(leftState.has_value() == false);
    }

    SECTION("transition to invalid state") {
        // given
        testStateMachine.setState(1);
        // when
        testStateMachine.changeState(9);
        // then
        REQUIRE(invalidState.has_value() == true);
        REQUIRE(invalidChange.has_value() == false);
        REQUIRE(enteredState.has_value() == false);
        REQUIRE(leftState.has_value() == false);
    }
}