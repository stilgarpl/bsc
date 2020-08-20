//
// Created by Krzysztof Tulidowicz on 20.07.18.
//

#ifndef BSC_STATEMACHINE_H
#define BSC_STATEMACHINE_H

#include <set>
#include <map>
#include <functional>
#include <core/log/Logger.h>
#include <atomic>


namespace bsc {
    template<typename StateIdType>
    class StateMachine {

        typedef StateMachine<StateIdType> StateMachineType;

    private:
        //@todo states and links should be moved to other class with defintion.
        std::set<StateIdType> states;
        std::map<StateIdType, std::set<StateIdType>> links;
        std::atomic<typename std::set<StateIdType>::iterator> currentState;
        std::function<void(const StateIdType&)> onEnterStateHandler;
        std::function<void(const StateIdType&)> onLeaveStateHandler;
        std::function<void(const StateIdType&)> invalidStateHandler;
        std::function<void(const StateIdType&)> invalidChangeHandler;
        std::recursive_mutex changeStateMutex;


    public:


    protected:

        auto getCurrentState() {
            //        std::lock_guard<std::recursive_mutex> g(changeStateMutex);
            return *currentState.load();
        }

    public:
        void addState(const StateIdType& state) {
            //        LOGGER("adding state" + std::to_string(state));
            states.insert(state);
        }

        template<typename ... StateIdTypes>
        void addState(const StateIdTypes& ... newStates) {
            StateIdType allStates[] = {newStates...};
            for (const auto& item : allStates) {
                addState(item);
            }
        }

        void addLink(const StateIdType& state1, const StateIdType& state2) {
            if (states.count(state1) > 0 && states.count(state2) > 0) {
                //            LOGGER("linking " + std::to_string(state1) + " to " + std::to_string(state2));
                links[state1].insert(state2);
            } else {
                //@todo exception?
            }
        }

        template<typename ... StateIdTypes>
        void addLink(const StateIdType& state1, const StateIdTypes& ... otherStates) {
            //@todo check if both states exist in states setDirect
            StateIdType allStates[] = {otherStates...};
            for (const auto& item : allStates) {
                addLink(state1, item);
            }
        }

    public:
        void setState(const StateIdType& state) {
            std::lock_guard<std::recursive_mutex> g(changeStateMutex);
            auto tempState = states.find(state);
            if (tempState == states.end()) {
                //@todo error handling
                abort();
            }
            currentState.store(tempState);
        }

        //@todo maybe return bool if change state was successful?
        //@todo maybe add method to check if changing state to other is valid and then run some code and do the change...
        void changeState(const StateIdType& state) {
            std::lock_guard<std::recursive_mutex> g(changeStateMutex);

            auto localCurrentState = this->currentState.load();
            //@todo error handling if currentState points to wrong state or uninitialized?
            //already in that state, do nothing.
            if (state == *localCurrentState) {
                //@todo think about it. Allow changing state to itself?
                return;
            }

            if (!states.count(state)) {
                invalidStateHandler(state);
                LOGGER("invalid state")
                return;
            }


            if (!links.count(*localCurrentState) || !links[*localCurrentState].count(state)) {
                invalidChangeHandler(state);
                LOGGER("invalid change")
                return;
            }

            if (localCurrentState != states.end()) {
                onLeaveStateHandler(*localCurrentState);
            }

            setState(state);
            onEnterStateHandler(state);
        }

    public:
        StateMachine(const std::function<void(const StateIdType&)>& onEnterStateHandler,
                     const std::function<void(const StateIdType&)>& onLeaveStateHandler) : onEnterStateHandler(
                onEnterStateHandler), onLeaveStateHandler(onLeaveStateHandler) {

        }

        StateMachine(const std::function<void(const StateIdType&)>& onEnterStateHandler,
                     const std::function<void(const StateIdType&)>& onLeaveStateHandler,
                     const std::function<void(const StateIdType&)>& invalidStateHandler,
                     const std::function<void(const StateIdType&)>& invalidChangeHandler) : onEnterStateHandler(
                onEnterStateHandler), onLeaveStateHandler(onLeaveStateHandler), invalidStateHandler(
                invalidStateHandler),
                                                                                            invalidChangeHandler(
                                                                                                    invalidChangeHandler) {}
    };
}


#endif //BSC_STATEMACHINE_H