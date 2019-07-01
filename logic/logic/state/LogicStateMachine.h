//
// Created by stilgar on 20.07.18.
//

#ifndef BASYCO_LOGICSTATEMACHINE_H
#define BASYCO_LOGICSTATEMACHINE_H


#include <core/log/Logger.h>
#include <logic/context/LogicContext.h>
#include <logic/sources/LogicStateSource.h>
#include "StateMachine.h"

template<typename StateObject, typename stateIdType>
class LogicStateMachine : public StateMachine<stateIdType> {
public:
    typedef stateIdType StateIdType;
    typedef LogicStateEvent<StateObject, stateIdType> EventType;
protected:
    typedef LogicStateMachine<StateObject, StateIdType> ThisType;
    std::recursive_mutex notifyLock;
public:

    class Observer {
    private:
        //@todo I don't like raw pointer here...
        std::recursive_mutex observeeMutex;
        //this should be set to null in observee destructor
        ThisType *observee = nullptr;

        void setObservee(ThisType *o) {
            std::lock_guard<std::recursive_mutex> g(observeeMutex);
            observee = o;
        }
    public:
        virtual void update(StateObject &object, StateIdType) = 0;

        virtual ~Observer() {
            std::lock_guard<std::recursive_mutex> g(observeeMutex);
            if (observee) {
                LOGGER("unregistering observer")
                observee->unregisterStateObserver(*this);
            }
        };

        friend class LogicStateMachine<StateObject, stateIdType>;
    };
private:
    StateObject &object;
    //Observer objects have to exist.
    std::list<std::reference_wrapper<Observer>> observers;

    void notify(StateIdType state) {
        notifyLock.lock();
        LOGGER(std::string("notify: ") + typeid(StateObject).name() + " : " + std::to_string(observers.size()))
        auto observersCopy = observers;
        notifyLock.unlock(); //@todo not sure if I can run update() without this locked. we'll see.
        for (const auto &observer : observersCopy) {
            auto &o = observer.get();
            o.update(object, state);
        }
    }

public:

    void registerStateObserver(Observer &observer) {
        std::lock_guard<std::recursive_mutex> g(notifyLock);
        observer.setObservee(this);
        observers.push_back(observer);
    }

    void unregisterStateObserver(Observer &observer) {
        std::lock_guard<std::recursive_mutex> g(notifyLock);
        LOGGER("unregistering observer ")
        observer.setObservee(nullptr);
        auto before = observers.size();
        observers.erase(std::remove_if(observers.begin(), observers.end(),
                                       [&](auto &i) -> bool {
                                           return (&i.get() == &observer);

                                       }), observers.end());
        auto after = observers.size();
        LOGGER("removed " + std::to_string(before - after))
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

    explicit LogicStateMachine(StateObject &object) : StateMachine<StateIdType>(
            std::bind(&LogicStateMachine<StateObject, StateIdType>::onEnter, this, std::placeholders::_1),
            std::bind(&LogicStateMachine<StateObject, StateIdType>::onLeave, this, std::placeholders::_1),
            std::bind(&LogicStateMachine<StateObject, StateIdType>::onInvalidState, this, std::placeholders::_1),
            std::bind(&LogicStateMachine<StateObject, StateIdType>::onInvalidChange, this, std::placeholders::_1)),
                                                      object(object) {

        //@todo handle no logic manager!
//        auto &lm = LogicContext::getLogicManagerFromActiveContext();
//        lm.setExecutionPolicy<EventType>(std::make_shared<OrderedExecutionPolicy>());
    }

    virtual ~LogicStateMachine() {
        auto observersCopy = observers;
        for (auto &observer : observersCopy) {
            observer.get().setObservee(nullptr);
        }
    }

};


#endif //BASYCO_LOGICSTATEMACHINE_H
