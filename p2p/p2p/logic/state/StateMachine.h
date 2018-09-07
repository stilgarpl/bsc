//
// Created by stilgar on 20.07.18.
//

#ifndef BASYCO_STATEMACHINE_H
#define BASYCO_STATEMACHINE_H

#include <set>
#include <map>
#include <functional>

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
    void addState(const StateIdType &state) {
        states.insert(state);
    }

    void addLink(const StateIdType &state1, const StateIdType &state2) {
        //@todo check if both states exist in states set
        links[state1].insert(state2);
    }

public:
    void setState(const StateIdType &state) {
        currentState = states.find(state);
    }

    void changeState(const StateIdType &state) {
        if (!states.count(state)) {
            invalidStateHandler(state);
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
            onEnterStateHandler), onLeaveStateHandler(onLeaveStateHandler) {}
};


#endif //BASYCO_STATEMACHINE_H
