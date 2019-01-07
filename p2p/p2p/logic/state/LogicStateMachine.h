//
// Created by stilgar on 20.07.18.
//

#ifndef BASYCO_LOGICSTATEMACHINE_H
#define BASYCO_LOGICSTATEMACHINE_H


#include <p2p/log/Logger.h>
#include <p2p/logic/LogicManager.h>
#include <p2p/node/context/LogicContext.h>
#include <p2p/logic/sources/LogicStateSource.h>
#include "StateMachine.h"

template<typename Object, typename stateIdType>
class LogicStateMachine : public StateMachine<stateIdType> {
public:
    typedef stateIdType StateIdType;
    typedef LogicStateEvent<Object, stateIdType> EventType;
protected:
    typedef LogicStateMachine<Object, StateIdType> ThisType;
public:

    class Observer {
    private:
        //@todo I don't like raw pointer here...
        ThisType *observee = nullptr;
    public:
        virtual void update(Object &object, StateIdType) = 0;

        virtual ~Observer() {
            if (observee) {
                observee->unregisterStateObserver(*this);
            }
        };
    };
private:
    Object &object;
    //Observer objects have to exist.
    std::list<std::reference_wrapper<Observer>> observers;

    void notify(StateIdType state) {
        for (const auto &observer : observers) {
            observer.get().update(object, state);
        }
    }

public:

    void registerStateObserver(Observer &observer) {
        observers.push_back(observer);
    }

    void unregisterStateObserver(Observer &observer) {
        std::remove_if(observers.begin(), observers.end(), [&](auto i) -> bool { return (&i.get() == &observer); });
    }


    void onEnter(const StateIdType &id) {
//        LOGGER("LOGIC MACHINE ON ENTER" + std::to_string(id));
        //@todo handle no logic manager!
        auto &lm = LogicContext::getLogicManagerFromActiveContext();
        auto &source = lm.requireSource<LogicStateSource>();
        source.stateEntered(object, id);
        notify(id);
    }

    void onLeave(const StateIdType &id) {
//        LOGGER("LOGIC MACHINE ON LEAVE" + std::to_string(id));
        //@todo handle no logic manager!
        auto &lm = LogicContext::getLogicManagerFromActiveContext();
        auto &source = lm.requireSource<LogicStateSource>();
        source.stateLeft(object, id);
    }

    void onInvalidChange(const StateIdType &id) {
//        LOGGER("LOGIC MACHINE ON INVALID CHANGE" + std::to_string(id));
    }

    void onInvalidState(const StateIdType &id) {
//        LOGGER("LOGIC MACHINE ON INVALID STATE" + std::to_string(id));
    }

//    LogicStateMachine() : StateMachine<StateIdType>(onEnter, onLeave) {};

    explicit LogicStateMachine(Object &object) : StateMachine<StateIdType>(
            std::bind(&LogicStateMachine<Object, StateIdType>::onEnter, this, std::placeholders::_1),
            std::bind(&LogicStateMachine<Object, StateIdType>::onLeave, this, std::placeholders::_1),
            std::bind(&LogicStateMachine<Object, StateIdType>::onInvalidState, this, std::placeholders::_1),
            std::bind(&LogicStateMachine<Object, StateIdType>::onInvalidChange, this, std::placeholders::_1)),
                                                 object(object) {

        //@todo handle no logic manager!
//        auto &lm = LogicContext::getLogicManagerFromActiveContext();
//        lm.setExecutionPolicy<EventType>(std::make_shared<OrderedExecutionPolicy>());
    }


};


#endif //BASYCO_LOGICSTATEMACHINE_H
