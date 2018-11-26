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

    class Definer {
    private:
        StateMachineType &stateMachine;
        const StateIdType &stateId;

    public:
        Definer(StateMachineType &stateMachine, const StateIdType &stateId) : stateMachine(stateMachine),
                                                                              stateId(stateId) {}

        Definer to(const StateIdType &otherState) {
            stateMachine.addState(otherState);
            stateMachine.addLink(stateId, otherState);
            return Definer(stateMachine, otherState);
        }

//        Definer operator()(const StateIdType& otherState) {
//            return to(otherState);
//        }
//
//        Definer& operator->() {
//            return *this;
//        }

    };

private:
    std::set <StateIdType> states;
    typename std::set<StateIdType>::iterator currentState;
    std::map <StateIdType, std::set<StateIdType>> links;
    std::function<void(const StateIdType &)> onEnterStateHandler;
    std::function<void(const StateIdType &)> onLeaveStateHandler;
    std::function<void(const StateIdType &)> invalidStateHandler;

    friend class Definer;

public:
    Definer define(const StateIdType &stateId) {
        addState(stateId);
        return Definer(*this, stateId);
    }

protected:
    //@todo remove
public:
    void addState(const StateIdType &state) {
        LOGGER("adding state" + std::to_string(state));
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
            LOGGER("linking " + std::to_string(state1) + " to " + std::to_string(state2));
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
        currentState = states.find(state);
    }

    void changeState(const StateIdType &state) {
        if (!states.count(state)) {
            invalidStateHandler(state);
        }
        //@todo check if there is a valid link!
        if (currentState != states.end()) {
            onLeaveStateHandler(*currentState);
        }
        setState(state);
        onEnterStateHandler(*currentState);
    }

public:
    StateMachine(const std::function<void(const StateIdType &)> &onEnterStateHandler,
                 const std::function<void(const StateIdType &)> &onLeaveStateHandler) : onEnterStateHandler(
            onEnterStateHandler), onLeaveStateHandler(onLeaveStateHandler) {}
};


#endif //BASYCO_STATEMACHINE_H
