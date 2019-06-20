//
// Created by stilgar on 20.07.18.
//

#ifndef BASYCO_STATEMACHINE_H
#define BASYCO_STATEMACHINE_H

#include <set>
#include <map>
#include <functional>
#include <p2p/log/Logger.h>

template<typename StateIdType>
class StateMachine {

    typedef StateMachine<StateIdType> StateMachineType;

private:
    //@todo states and links should be moved to other class with defintion.
    std::set <StateIdType> states;
    std::map <StateIdType, std::set<StateIdType>> links;
    typename std::set<StateIdType>::iterator currentState;
    std::function<void(const StateIdType &)> onEnterStateHandler;
    std::function<void(const StateIdType &)> onLeaveStateHandler;
    std::function<void(const StateIdType &)> invalidStateHandler;
    std::function<void(const StateIdType &)> invalidChangeHandler;
    std::recursive_mutex changeStateMutex;


public:


protected:

    auto getCurrentState() {
        return *currentState;
    }

public:
    void addState(const StateIdType &state) {
//        LOGGER("adding state" + std::to_string(state));
        states.insert(state);
    }

    template<typename ... StateIdTypes>
    void addState(const StateIdTypes &... newStates) {
        StateIdType allStates[] = {newStates...};
        for (const auto &item : allStates) {
            addState(item);
        }
    }

    void addLink(const StateIdType &state1, const StateIdType &state2) {
        if (states.count(state1) > 0 && states.count(state2) > 0) {
//            LOGGER("linking " + std::to_string(state1) + " to " + std::to_string(state2));
            links[state1].insert(state2);
        } else {
            //@todo exception?
        }
    }

    template<typename ... StateIdTypes>
    void addLink(const StateIdType &state1, const StateIdTypes &... otherStates) {
        //@todo check if both states exist in states set
        StateIdType allStates[] = {otherStates...};
        for (const auto &item : allStates) {
            addLink(state1, item);
        }
    }
public:
    void setState(const StateIdType &state) {
        std::lock_guard<std::recursive_mutex> g(changeStateMutex);
        currentState = states.find(state);
        if (currentState == states.end()) {
            //@todo error handling
            abort();
        }
    }

    //@todo maybe return bool if change state was successful?
    //@todo maybe add method to check if changing state to other is valid and then run some code and do the change...
    void changeState(const StateIdType &state) {
        std::lock_guard<std::recursive_mutex> g(changeStateMutex);
        //already in that state, do nothing.
        //@todo error handling if currentState points to wrong state or uninitialized?
        if (state == *currentState) {
            //@todo think about it. Allow changing state to itself?
            return;
        }

        if (!states.count(state)) {
            invalidStateHandler(state);
            LOGGER("invalid state")
            return;
        }


        if (!links.count(*currentState) || !links[*currentState].count(state)) {
            invalidChangeHandler(state);
            LOGGER("invalid change")
            return;
        }

        if (currentState != states.end()) {
            onLeaveStateHandler(*currentState);
        }

        setState(state);
        onEnterStateHandler(*currentState);
    }

public:
    StateMachine(const std::function<void(const StateIdType &)> &onEnterStateHandler,
                 const std::function<void(const StateIdType &)> &onLeaveStateHandler) : onEnterStateHandler(
            onEnterStateHandler), onLeaveStateHandler(onLeaveStateHandler) {

    }

    StateMachine(const std::function<void(const StateIdType &)> &onEnterStateHandler,
                 const std::function<void(const StateIdType &)> &onLeaveStateHandler,
                 const std::function<void(const StateIdType &)> &invalidStateHandler,
                 const std::function<void(const StateIdType &)> &invalidChangeHandler) : onEnterStateHandler(
            onEnterStateHandler), onLeaveStateHandler(onLeaveStateHandler), invalidStateHandler(invalidStateHandler),
                                                                                         invalidChangeHandler(
                                                                                                 invalidChangeHandler) {}
};


#endif //BASYCO_STATEMACHINE_H
