//
// Created by Krzysztof Tulidowicz on 20.07.18.
//

#ifndef BSC_LOGICSTATEMACHINE_H
#define BSC_LOGICSTATEMACHINE_H

#include "StateMachine.h"
#include "StateObserver.h"
#include <core/log/Logger.h>
#include <core/thread/ThreadQueueProcessor.h>
#include <logic/context/LogicContext.h>
#include <logic/sources/LogicStateSource.h>

namespace bsc {
    template<typename StateObject, typename stateIdType, template<typename, typename> typename NotifyMethodType>
    class LogicStateMachine : public StateMachine<stateIdType>, public Observee<StateObject, stateIdType> {
    public:
        using StateIdType = stateIdType;
        using EventType   = LogicStateEvent<StateObject, stateIdType>;

    protected:
        using ThisType     = LogicStateMachine<StateObject, StateIdType, NotifyMethodType>;
        using NotifyMethod = NotifyMethodType<StateObject, StateIdType>;

    public:
        using ObserverType = Observer<StateObject, StateIdType>;

    private:
        StateObject& object;
        // ObserverType objects have to exist.
        std::unique_ptr<NotifyMethod> notifyMethod;

        void notify(StateIdType state) { notifyMethod->notify(object, state); }

    public:
        void registerStateObserver(ObserverType& observer) {
            observer.setObservee(this);
            notifyMethod->registerObserver(observer);
        }

        void unregisterStateObserver(ObserverType& observer) {

            LOGGER("unregistering observer ")
            observer.setObservee(nullptr);
            notifyMethod->unregisterObserver(observer);
        }

        void onEnter(const StateIdType& id) {
            //        LOGGER("LOGIC MACHINE ON ENTER" + std::to_string(id));
            //@todo handle no logic manager!
            auto& lm     = LogicContext::getLogicManagerFromActiveContext();
            auto& source = lm.requireSource<LogicStateSource>();
            source.stateEntered(object, id);
            notify(id);
        }

        void onLeave(const StateIdType& id) {
            //        LOGGER("LOGIC MACHINE ON LEAVE" + std::to_string(id));
            //@todo handle no logic manager!
            auto& lm     = LogicContext::getLogicManagerFromActiveContext();
            auto& source = lm.requireSource<LogicStateSource>();
            source.stateLeft(object, id);
        }

        void onInvalidChange(const StateIdType& id) {
            //        LOGGER("LOGIC MACHINE ON invalid CHANGE" + std::to_string(id));
        }

        void onInvalidState(const StateIdType& id) {
            //        LOGGER("LOGIC MACHINE ON invalid STATE" + std::to_string(id));
        }

        //    LogicStateMachine() : StateMachine<StateIdType>(onEnter, onLeave) {};

        LogicStateMachine(typename LogicStateMachine::DefinitionPtr definition, StateObject& object)
            : StateMachine<StateIdType>(definition,
                                        std::bind(&ThisType::onEnter, this, std::placeholders::_1),
                                        std::bind(&ThisType::onLeave, this, std::placeholders::_1),
                                        std::bind(&ThisType::onInvalidState, this, std::placeholders::_1),
                                        std::bind(&ThisType::onInvalidChange, this, std::placeholders::_1)),
              object(object) {

            //@todo handle no logic manager!
            //        auto &lm = LogicContext::getLogicManagerFromActiveContext();
            //        lm.setExecutionPolicy<EventType>(std::make_shared<OrderedExecutionPolicy>());
            notifyMethod = std::make_unique<NotifyMethod>();
        }

        virtual ~LogicStateMachine() {
            // this should perform cleanup of observers held in notifyMethod. may join notifyMethod thread if threaded
            notifyMethod = nullptr;
        }
    };
}// namespace bsc

#endif// BSC_LOGICSTATEMACHINE_H
