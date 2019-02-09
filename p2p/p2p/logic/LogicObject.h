#include <utility>

//
// Created by stilgar on 06.07.18.
//

#ifndef BASYCO_LOGICOBJECT_H
#define BASYCO_LOGICOBJECT_H


#include "LogicManager.h"

#include <p2p/logic/sources/AutoSource.h>
#include <p2p/logic/events/LogicStateEvent.h>
#include <p2p/logic/events/ChainEvent.h>
#include <p2p/logic/events/EventWrapper.h>
#include <p2p/logic/events/Tick.h>

class LogicObject {
protected:
    LogicManager &logicManager;

public:
    explicit LogicObject(LogicManager &logicManager);

public:

    class SetupActionHelper {
        LogicManager &logicManager;
    public:
        explicit SetupActionHelper(LogicManager &logicManager) : logicManager(logicManager) {}

    public:
        template<typename... Args, typename ActionIdType>
        void setAction(ActionIdType id, ActionManager::ActionType<Args...> action) {
            logicManager.setAction<Args...>(id, action);
        };

        template<typename... Args, typename ActionIdType, typename RetType>
        void setActionExtended(ActionIdType id, std::function<RetType(Args...)> func) {
            logicManager.setActionExtended(id, func);
        }
    };

    class AssignActionHelper {
        LogicManager &logicManager;
    public:
        explicit AssignActionHelper(LogicManager &logicManager) : logicManager(logicManager) {}

    public:
        template<typename EventType, typename... Args, typename ActionId>
        bool assignAction(typename EventType::IdType eventId, ActionId actionId) {
            return logicManager.assignAction<EventType, Args...>(eventId, actionId);
        }

        template<typename EventType, typename... Args, typename ActionId>
        bool assignAction(ActionId actionId) {
            return logicManager.assignAction<EventType, Args...>(actionId);
        }
    };

    class SetupSourceHelper {
        LogicManager &logicManager;
    public:
        explicit SetupSourceHelper(LogicManager &logicManager) : logicManager(logicManager) {}

    public:
        template<typename SourceType, typename... Args>
        void requireSource(Args... args) {
            logicManager.requireSource<SourceType, Args...>(args...);
        }
    };

    template<typename EventType, typename ... Args>
    class LogicChainHelper;

    template<typename eventType, typename ... Args>
    class EventHelper {
    public:
        typedef eventType EventType;
        typedef EventHelper<eventType, Args...> ThisType;
        typedef typename EventType::IdType EventId;
        typedef std::function<bool(EventType &)> ConstraintFunc;

    private:
        std::optional<EventId> eventId;
        std::list<ConstraintFunc> _constraint;
    public:
        EventHelper<EventType, Args...> &withId(EventId id) {
            eventId = id;
            return *this;
        }

        EventType makeEvent() {
            EventType event;
            if (eventId) {
                event.setEventId(*eventId);
            }
            return event;
        }

        EventHelper() = default;

        explicit EventHelper(const std::optional<EventId> &eventId) : eventId(eventId) {}


        ThisType &constraint(const ConstraintFunc &func) {
            _constraint.push_back(func);
//            LOGGER("adding constraint, size " + std::to_string(_constraint.size()))
            return *this;
        }

        friend class LogicChainHelper<EventType, Args...>;
    };

    template<typename eventType, typename ... Args>
    class StateHelper : public EventHelper<eventType, Args...> {
        // typedef StateHelper<eventType,Args...> ThisType;

    public:
        StateHelper<eventType, Args...> &entered() {
            this->constraint(
                    [](eventType &event) -> bool { return event.getMethod() == LogicStateMethod::ENTERED; });
            return *this;
        }

        StateHelper<eventType, Args...> &left() {
            this->constraint(
                    [](eventType &event) -> bool { return event.getMethod() == LogicStateMethod::LEFT; });
            return *this;
        }

        StateHelper() = default;

        explicit StateHelper(const std::optional<typename EventHelper<eventType, Args...>::EventId> &eventId)
                : EventHelper<eventType, Args...>(eventId) {}
    };


    template<typename EventType, typename ... Args>
    class LogicChainHelper {
    public:
        typedef typename EventType::IdType EventId;
        typedef LogicChainHelper<EventType, Args...> ThisType;
        typedef typename EventHelper<EventType, Args...>::ConstraintFunc ConstraintFunc;
    private:
        std::optional<EventId> eventId;
        std::optional<ChainIdType> chainId = std::nullopt;
        int childId = 0;

        std::list<ConstraintFunc> _constraint;
        LogicManager &logicManager;
    public:
        explicit LogicChainHelper(const EventHelper<EventType, Args...> &eventHelper, LogicManager &l) : logicManager(
                l) {
            eventId = eventHelper.eventId;
            _constraint = eventHelper._constraint;
        }

        explicit LogicChainHelper(const EventHelper<EventType, Args...> &eventHelper, LogicManager &l, ChainIdType id)
                : LogicChainHelper(eventHelper, l) {
            chainId = id;
        };



        ThisType &constraint(const ConstraintFunc &func) {
            _constraint.push_back(func);
            return *this;
        }

        template<typename ActionId>
        ThisType &fireAction(ActionId actionId) {
            std::cout << "assigning action ... " << std::endl;
            bool success;

            //@todo if constrained then event<ConstrainedActionEvent> -> actionId; actionId = constrainedActionId

            if (!_constraint.empty()) {

                //@todo what if no action?
                auto action = logicManager.getAction<EventType, Args...>(actionId);
                //action is constrained, wrap it in check action
                if (action) {
                    auto generatedActionId = generateActionId<unsigned long>();
                    auto checkAction = [=, constraint = _constraint](EventType e, Args... args) {
                        bool result = true;
                        for (auto &i : constraint) {
                            result &= ((i)(e));
                        }

                        if (result) {
                            (*action)(e, args...);
                        }
                    };
                    logicManager.setAction<EventType, Args...>(generatedActionId, checkAction);
                    //@todo combine duplicated branches if possible
                    if (eventId) {
                        success = logicManager.assignAction<EventType, Args...>(*eventId, generatedActionId);
                    } else {

                        success = logicManager.assignAction<EventType, Args...>(generatedActionId);
                    }
                } else {
                    success = false;
                }


            } else {

                //@todo throw some exception or sth if action isn't found
                if (eventId) {
                    success = logicManager.assignAction<EventType, Args...>(*eventId, actionId);
                } else {

                    success = logicManager.assignAction<EventType, Args...>(actionId);
                }
            }
            if (!success) {
                throw std::string("Failed to assign action!");
            }
            return *this;
        }

        template<typename ActionId>
        auto generateActionId() {
            //@todo maybe create a version that will also work with strings? I have a lot of strings...
            static ActionId generatedActionId = 1;
            return generatedActionId++;
        }

        auto nextChildId() {
            return childId++;
        }

        auto nextChainId() {
            const std::string SEPARATOR = ".";
            return *chainId + SEPARATOR + std::to_string(nextChildId());
        }

        ThisType &fireNewAction(ActionManager::ActionType<EventType, Args...> action) {

            auto generatedActionId = generateActionId<unsigned long>();

            std::cout << "assigning action with generated id " << std::to_string(generatedActionId) << std::endl;
            logicManager.setAction<EventType, Args...>(generatedActionId, action);
            fireAction(generatedActionId);
            return *this;
        }

        template<typename GenericFunc, typename ... Evaluators>
        auto fireNewGenericAction(GenericFunc genericFunc, Evaluators... evaluators) {
            LOGGER("fireNewGenericAction - normal")
            return fireNewAction([=](EventType e) {
                genericFunc(evaluators(e)...);
            });

        }

        template<typename GenericFunc,template<typename E> typename ... Evaluators>
        auto fireNewGenericChainAction(GenericFunc genericFunc, Evaluators<EventType>... evaluators) {
            LOGGER("fireNewGenericChainAction - template")
            //@todo check if RetType is void or already an event type
            using RetType = std::invoke_result_t<GenericFunc, std::invoke_result_t<Evaluators<EventType>, EventType, Args...>...>;
            return fireNewChainAction([=](EventType e) {
                return EventWrapper<RetType>(genericFunc(evaluators(e)...));
            });

        }

        /*
         * works like generic func, but return value should be bool or convertible to bool
         */
        template<typename ConditionalFunc, template<typename E> typename ... Evaluators>
        auto conditional(ConditionalFunc conditionalFunc, bool value, Evaluators<EventType>... evaluators) {
            //@todo check if RetType is void or already an event type
            using RetType = std::invoke_result_t<ConditionalFunc, std::invoke_result_t<Evaluators<EventType>, EventType, Args...>...>;
            return fireNewChainAction([=](EventType e) {
                EventWrapper<RetType> ret(conditionalFunc(evaluators(e)...));
                if (ret != value) {
                    ret.setEventValid(false);
                }
                return ret;
            });
        }

        template<typename ConditionalFunc, template<typename E> typename ... Evaluators>
        auto ifTrue(ConditionalFunc conditionalFunc, bool value, Evaluators<EventType>... evaluators) {
            return conditional(conditionalFunc, true, evaluators...);
        }

        template<typename ConditionalFunc, template<typename E> typename ... Evaluators>
        auto ifFalse(ConditionalFunc conditionalFunc, bool value, Evaluators<EventType>... evaluators) {
            return conditional(conditionalFunc, false, evaluators...);
        }

        template<typename GenericFunc, typename ... Evaluators>
        auto fireNewGenericChainAction(GenericFunc genericFunc, Evaluators... evaluators) {
            LOGGER("fireNewGenericChainAction - normal")
            //@todo check if RetType is void or already an event type
            using RetType = std::invoke_result_t<GenericFunc, std::invoke_result_t<Evaluators, EventType, Args...>...>;
            if constexpr (std::is_same<void, RetType>::value) {
                return fireNewChainAction([=](EventType e) {
                    genericFunc(evaluators(e)...);
                });
            } else {
                return fireNewChainAction([=](EventType e) {
                    return EventWrapper<RetType>(genericFunc(evaluators(e)...));
                });
            }

        }

        //@todo func or std::function<RetType(EventType,Args...)> ?
        template<typename Func, typename ... SetArgs>
        auto fireNewChainAction(Func func, SetArgs... setArgs) {
            using RetType = std::invoke_result_t<Func, EventType, SetArgs...>;
            if (chainId) {
                auto generatedActionId = generateActionId<unsigned long>();
                auto generatedChainId = nextChainId();
                //@todo apply constraint?
                if constexpr (!std::is_same<void, RetType>::value) {
                    std::function<ChainEvent<RetType>(ChainEvent<EventType>)> f = [=, chainId = chainId](
                            ChainEvent<EventType> chainedEvent) {
                        LOGGER("chain action " + (chainId ? *chainId : "NO CHAIN ID") + " stage : " +
                               chainedEvent.getStageId())

                        if (chainedEvent.getActualEvent() && *chainId == chainedEvent.getStageId()) {
                            LOGGER("chained action" + *chainId)
                            ChainEvent<RetType> result(generatedChainId,
                                                       func(*chainedEvent.getActualEvent(), setArgs...));
                            return result;
                        } else {
                            return ChainEvent<RetType>(); //it has empty actual event and should stop the chain.
                        }

                    };
                    LOGGER("generated chain id " + generatedChainId);
                    //@todo if RetType == void, then setAction, not ActionExtended.
                    logicManager.setActionExtended<ChainEvent<RetType>, ChainEvent<EventType>, Args...>(
                            generatedActionId,
                            f);
                    logicManager.assignAction<ChainEvent<EventType>, Args...>(generatedActionId);
                    //@todo return LogicChainHelper for <RetType>, but what about Args... ?
                    LogicChainHelper<RetType> retLogicHelper(EventHelper<RetType>(), logicManager, generatedChainId);
                    return retLogicHelper;
                } else {

                    std::function<void(ChainEvent<EventType>)> f = [=, chainId = chainId](
                            ChainEvent<EventType> chainedEvent) {
                        LOGGER("chain action " + (chainId ? *chainId : "NO CHAIN ID") + " stage : " +
                               chainedEvent.getStageId())

                        if (chainedEvent.getActualEvent() && *chainId == chainedEvent.getStageId()) {
                            func(*chainedEvent.getActualEvent(), setArgs...);
                        }

                    };


                    logicManager.setAction<ChainEvent<EventType>, Args...>(
                            generatedActionId, f);
                    logicManager.assignAction<ChainEvent<EventType>, Args...>(generatedActionId);
                    return *this;
                }

            } else {

                //@todo exception!!!
                LOGGER("FAILURE! NO EVENT CHAIN ID SET!")
                //@todo generated chain if no chain set! fix!
                auto generatedChainId = nextChainId();
                if constexpr (!std::is_same<void, RetType>::value) {
                    LogicChainHelper<RetType> retLogicHelper(EventHelper<RetType>(), logicManager, generatedChainId);
                    return retLogicHelper;
                } else {
                    return *this;
                }
            }

        }

        ThisType &newChain(ChainIdType id) {
            chainId = id;
            transform<ChainEvent<EventType>>([=](auto event) {
                LOGGER("starting chain " + id)
                ChainEvent r(id, event);
                return r;
            });
            return *this;
        }

        template<typename NewEventType, typename Func>
        ThisType &transform(Func transformer) {
            auto generatedActionId = generateActionId<unsigned long>();
            std::function<NewEventType(EventType)> f = [=](EventType event) -> NewEventType {
                return transformer(event);
            };
            std::cout << "transforming action with generated id " << std::to_string(generatedActionId) << std::endl;
            logicManager.setActionExtended<NewEventType, EventType, Args...>(generatedActionId, f);
            fireAction(generatedActionId);
            return *this;
        }


        template<typename NewEventType, typename ... NewArgs>
        ThisType &emit() {
            //@todo initialize some actual values.
//            logicManager.event<NewEventType,NewArgs...>(NewEventType(),NewArgs()...);
            auto &autoSource = logicManager.requireSource<AutoSource>();
            fireNewAction([&](EventType e, Args...) {
                autoSource.template generateEvent<NewEventType>();
            });


            return *this;
        };

        template<typename NewEventType, typename ... NewArgs>
        ThisType &emit(NewEventType newEventType) {
            //@todo initialize some actual values.
//            logicManager.event<NewEventType,NewArgs...>(NewEventType(),NewArgs()...);
            auto &autoSource = logicManager.requireSource<AutoSource>();
            fireNewAction([&, newEventType](EventType e, Args...) {
                autoSource.template generateEvent<NewEventType>(newEventType);
            });

            return *this;
        };

        template<typename NewEventType>
        ThisType &emit(EventHelper<NewEventType> eventHelper) {
            emit(eventHelper.makeEvent());
        }

//        template<typename ModuleType, typename ... Args>
//        ThisType& runModuleMethod() {
//
//        };

    };

    template<typename EventType, typename ... Args>
    class SpecificLogicChainHelper : public LogicChainHelper<EventType, Args...> {
    public:
        explicit SpecificLogicChainHelper(const EventHelper<EventType, Args...> &eventHelper, LogicManager &l)
                : LogicChainHelper<EventType, Args...>(eventHelper, l) {};
    };

    template<typename Object, typename StateIdType, typename ... Args>
    class SpecificLogicChainHelper<LogicStateEvent<Object, StateIdType>, Args...>
            : public LogicChainHelper<LogicStateEvent<Object, StateIdType>, Args...> {
        typedef LogicStateEvent<Object, StateIdType> LogicEventType;
        typedef SpecificLogicChainHelper<LogicStateEvent<Object, StateIdType>> ThisType;
    public:
        explicit SpecificLogicChainHelper(const EventHelper<LogicEventType, Args...> &eventHelper, LogicManager &l)
                : LogicChainHelper<LogicEventType, Args...>(eventHelper, l) {};

//        void valid() {};
        typename LogicChainHelper<LogicEventType, Args...>::ThisType &
        fireStateChangeReaction(std::function<void(typename LogicEventType::Type &)> reaction) {
            return LogicChainHelper<LogicEventType, Args...>::fireNewAction(
                    [reaction](auto event) {
                        reaction(event.getObject());
                    });
        }

        typename LogicChainHelper<LogicEventType, Args...>::ThisType &
        fireStateChangeReaction(std::function<void(typename LogicEventType::Type &, const StateIdType &)> reaction) {
            return LogicChainHelper<LogicEventType, Args...>::fireNewAction(
                    [reaction](auto event) { reaction(event.getObject(), event.getState()); });
        }
    };


    template<typename EventType, typename ... Args>
    static EventHelper<EventType, Args...> event() {
        return EventHelper<EventType, Args...>();
    }

    template<typename ObjectType, typename StateIdType = typename ObjectType::StateIdType, typename ... Args>
    StateHelper<LogicStateEvent<ObjectType, StateIdType>, Args...> state() {
        return StateHelper<LogicStateEvent<ObjectType, StateIdType>, Args...>();
    }

    template<typename ObjectType, typename StateIdType = typename ObjectType::StateIdType, typename ... Args>
    StateHelper<LogicStateEvent<ObjectType, StateIdType>, Args...> state(StateIdType id) {
        return StateHelper<LogicStateEvent<ObjectType, StateIdType>, Args...>(id);
    }

    template<typename EventType, typename ... Args>
    static EventHelper<EventType, Args...> event(typename EventType::IdType id) {
        return EventHelper<EventType, Args...>(id);
    }

    template<typename EventType, typename ... Args>
    SpecificLogicChainHelper<EventType, Args...> when(const EventHelper<EventType, Args...> eventHelper) {
        return SpecificLogicChainHelper<EventType, Args...>(eventHelper, logicManager);
    }


public:
    virtual bool setupLogic() {
        bool ret = true;
        SetupSourceHelper setupSourceHelper(logicManager);
        SetupActionHelper setupActionHelper(logicManager);
        AssignActionHelper assignActionHelper(logicManager);
        ret &= setupSources(setupSourceHelper);
        setupActions(setupActionHelper);
        ret &= assignActions(assignActionHelper);
        return ret;
    };

    virtual void setupActions(SetupActionHelper &actionHelper) = 0;

    virtual bool assignActions(AssignActionHelper &actionHelper)=0;

    virtual bool setupSources(SetupSourceHelper &sourceHelper)=0;

};


#endif //BASYCO_LOGICOBJECT_H
