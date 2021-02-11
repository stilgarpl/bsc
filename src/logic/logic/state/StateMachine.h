//
// Created by Krzysztof Tulidowicz on 20.07.18.
//

#ifndef BSC_STATEMACHINE_H
#define BSC_STATEMACHINE_H

#include "StateMachineDefinition.h"
#include <atomic>
#include <functional>
#include <log/log/Logger.h>
#include <map>
#include <set>

namespace bsc {

    template<typename StateIdType>
    class StateMachine {
    public:
        using StateMachineType = StateMachine<StateIdType>;
        using Definition       = StateMachineDefinition<StateIdType>;
        using DefinitionPtr    = std::shared_ptr<const Definition>;

        class StateMachineUninitialized : public std::domain_error {
        public:
            explicit StateMachineUninitialized(const std::string& arg) : domain_error(arg) {}
        };

        class InvalidStateMachineDefinition : public std::domain_error {
        public:
            explicit InvalidStateMachineDefinition(const std::string& arg) : domain_error(arg) {}
        };

    private:
        const DefinitionPtr definition;
        std::atomic<const StateIdType*> currentState = nullptr;
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
        void setState(const StateIdType& state) {
            std::lock_guard<std::recursive_mutex> g(changeStateMutex);
            currentState.store(&definition->getState(state));
        }

        //@todo maybe return bool if change state was successful?
        //@todo maybe add method to check if changing state to other is valid and then run some code and do the change...
        void changeState(const StateIdType& state) {
            std::lock_guard<std::recursive_mutex> g(changeStateMutex);

            auto localCurrentState = this->currentState.load();

            if (localCurrentState == nullptr) {
                ERROR("state machine is in invalid state")
                throw StateMachineUninitialized("state machine is in invalid state");
            }

            // already in that state, do nothing.
            if (state == *localCurrentState) {
                //@todo think about it. Allow changing state to itself?
                return;
            }

            if (!definition->hasState(state)) {
                invalidStateHandler(state);
                LOGGER("invalid state")
                return;
            }

            if (!definition->hasLink(*localCurrentState, state)) {
                invalidChangeHandler(state);
                LOGGER("invalid change")
                return;
            }

            onLeaveStateHandler(*localCurrentState);

            setState(state);
            onEnterStateHandler(state);
        }

    public:
        StateMachine(const DefinitionPtr definition,
                     const std::function<void(const StateIdType&)>& onEnterStateHandler,
                     const std::function<void(const StateIdType&)>& onLeaveStateHandler,
                     const std::function<void(const StateIdType&)>& invalidStateHandler,
                     const std::function<void(const StateIdType&)>& invalidChangeHandler)
            : definition(definition), onEnterStateHandler(onEnterStateHandler), onLeaveStateHandler(onLeaveStateHandler),
              invalidStateHandler(invalidStateHandler), invalidChangeHandler(invalidChangeHandler) {
            if (definition == nullptr) {
                throw InvalidStateMachineDefinition("Invalid state machine definition");
            }
        }

        StateMachine(const DefinitionPtr definition,
                     const std::function<void(const StateIdType&)>& onEnterStateHandler,
                     const std::function<void(const StateIdType&)>& onLeaveStateHandler)
            : StateMachine(definition, onEnterStateHandler, onLeaveStateHandler) {}
    };
}// namespace bsc

#endif// BSC_STATEMACHINE_H
