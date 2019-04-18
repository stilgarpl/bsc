#include <utility>

#include <utility>

//
// Created by stilgar on 06.07.18.
//

#ifndef BASYCO_LOGICOBJECT_H
#define BASYCO_LOGICOBJECT_H


#include "LogicManager.h"
#include <p2p/logic/sources/AutoSource.h>
#include <p2p/logic/events/LogicStateEvent.h>
#include <p2p/logic/chain/ChainEvent.h>
#include <p2p/logic/events/EventWrapper.h>
#include <p2p/logic/events/Tick.h>
#include <p2p/logic/chain/ChainContext.h>
#include <p2p/logic/chain/GlobalChainContext.h>
#include <p2p/logic/chain/ChainGroup.h>
#include <p2p/logic/chain/LockConfiguration.h>

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
        }

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


    template<typename eventType, typename ... Args>
    class LogicChainHelper {
    public:
        using EventType = eventType;
        typedef typename EventType::IdType EventId;
        typedef LogicChainHelper<EventType, Args...> ThisType;
        typedef typename EventHelper<EventType, Args...>::ConstraintFunc ConstraintFunc;
    private:
        std::optional<EventId> eventId;
        std::optional<ChainIdType> chainId = std::nullopt;
    public:
        const std::optional<ChainIdType> &getChainId() const {
            //@todo return this or optional?
            return chainId;
        }

    private:
        int childId = 0;

        std::list<ConstraintFunc> _constraint;
        LogicManager &logicManager;


        static void obtainChainLock(const ChainLockIdType &chainLockId, InstanceType instance, bool lock) {
            //@todo mutex synchronize this whole method? on mutex from chainLock?

            auto chainContext = Context::getActiveContext()->get<GlobalChainContext>();
            auto &chainLock = chainContext->getChainLock(chainLockId);
            //@todo maybe use different mutex than chainLock mutex?
            std::unique_lock<std::recursive_mutex> guard(chainLock.getMutex());
            if (chainLock.isLocked()) {
                if (*chainLock.getInstance() != instance) {
                    //@todo remove mutex unlocking, maybe add parameter to waitForUnlock to double unlock it?
                    guard.unlock();
                    chainLock.waitForUnlock();
                    guard.lock();
                }
            }
            if (lock) {
                chainLock.lock(instance);
            }
            LOGGER("LOCK OBTAINED")
        }

        static void releaseChainLock(const ChainIdType &chainId, InstanceType instance) {
            //@todo mutex synchronize this whole method? on mutex from chainLock?
            auto chainContext = Context::getActiveContext()->get<GlobalChainContext>();
            auto &chainLock = chainContext->getChainLock(chainId);
            LOGGER("RELEASE BEFORE")
            std::unique_lock<std::recursive_mutex> guard(chainLock.getMutex());
            //@todo check instance.
            if (chainLock.isLocked()) {
                chainLock.unlock();
            }
            LOGGER("RELEASE AFTER")
        }

    public:
        explicit LogicChainHelper(const EventHelper<EventType, Args...> &eventHelper, LogicManager &l) : logicManager(
                l) {
            eventId = eventHelper.eventId;
            _constraint = eventHelper._constraint;
        }

        explicit LogicChainHelper(const EventHelper<EventType, Args...> &eventHelper, LogicManager &l, const ChainIdType& id)
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
//            int status;
//            auto realname = abi::__cxa_demangle(typeid(EventType).name(), 0, 0, &status);

//            LOGGER("fire new action for type " + std::string(typeid(EventType).name()) + " .... " + realname )
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

        //@todo remove eventually if not needed
//        template<typename GenericFunc, template<typename E> typename ... Evaluators>
//        auto fireNewGenericChainAction(GenericFunc genericFunc, Evaluators<EventType>... evaluators) {
//            LOGGER("fireNewGenericChainAction - template")
//            //@todo check if RetType is void or already an event type
//            using RetType = std::invoke_result_t<GenericFunc, std::invoke_result_t<Evaluators<EventType>, EventType, Args...>...>;
//            return fireNewChainAction([=](EventType e) {
//                //@todo should I set eventWrapper ID to something? I don't think it's ever fired as a standalone event, not wrapped in chainEvent...
//                return EventWrapper<RetType>(genericFunc(evaluators(e)...));
//            });
//
//        }




        template<typename GenericFunc, typename ... Evaluators>
        auto fireNewGenericChainAction(GenericFunc genericFunc, Evaluators... evaluators) {
            LOGGER("fireNewGenericChainAction - normal")
            //@todo check if RetType is void or already an event type
            using RetType = std::invoke_result_t<GenericFunc, std::invoke_result_t<Evaluators, EventType, Args...>...>;
            if constexpr (std::is_same<void, RetType>::value) {
                return fireNewChainAction([=](EventType e) {
                    genericFunc(evaluators(e)...);
                    return EventWrapper<RetType>();
                });
            } else {
                return fireNewChainAction([=](EventType e) {
                    return EventWrapper<RetType>(genericFunc(evaluators(e)...));
                });
            }


        }


//        //@todo maybe think of better name
//        template<typename ConditionalFunc, typename ValueType, typename ... Evaluators>
//        auto ifChain(ConditionalFunc conditionalFunc, ValueType value, Evaluators... evaluators) {
//            //this method will emit two events, one for result == value, the other for result != value
//            using RetType = std::invoke_result_t<ConditionalFunc, std::invoke_result_t<Evaluators, EventType, Args...>...>;
//            //@todo make typedef for std::list
//            std::function<std::list<ChainEvent<RetType>>(ChainEvent<EventType>)> f = [](){};
//            return fireRawChainActionPack<std::list,RetType>(f,);
//        }


        /*
         * works like generic func, but return value should be bool or convertible to bool
         */
        template<typename ConditionalFunc, typename ValueType, typename ... Evaluators>
        auto conditionalChain(ConditionalFunc conditionalFunc, ValueType value, Evaluators... evaluators) {
            //@todo check if RetType is void or already an event type
            using RetType = std::invoke_result_t<ConditionalFunc, std::invoke_result_t<Evaluators, EventType, Args...>...>;

            //RetType should be comparable to ValueType
            return fireNewChainAction([=](EventType e) {
                //@todo what if ValueType != RetType?
                ConditionalEventWrapper<RetType, ValueType> ret(conditionalFunc(evaluators(e)...), value);

//                if (ret.getPayload() != value) {
//                    //terminates the chain
//                    ret.setEventValid(false);
//                }
                return ret;
            });
        }

        template<typename ConditionalFunc, typename ... Evaluators>
        auto ifTrue(ConditionalFunc conditionalFunc, Evaluators... evaluators) {
            return conditionalChain(conditionalFunc, true, evaluators...);
        }

        template<typename ConditionalFunc, typename ... Evaluators>
        auto ifFalse(ConditionalFunc conditionalFunc, Evaluators... evaluators) {
            return conditionalChain(conditionalFunc, false, evaluators...);
        }

        template<typename ValueType = bool>
        auto thenChain() {
            return fireNewChainAction([=](EventType e) {
                if (e.getPayload() != e.getOtherValue()) {
                    //terminates the chain
                    e.setEventValid(false);
                }
                return e;
            });
        }

        template<typename ValueType = bool>
        auto elseChain() {
            return fireNewChainAction([=](EventType e) {
                if (e.getPayload() == e.getOtherValue()) {
                    //terminates the chain
                    e.setEventValid(false);
                }
                return e;
            });
        }


        auto lockChain() {
            auto generatedActionId = generateActionId<unsigned long>();
            auto generatedChainId = nextChainId();
            std::function<ChainEvent<EventType>(ChainEvent<EventType>)> f =
                    [generatedChainId](ChainEvent<EventType> event, auto... args) -> ChainEvent<EventType> {
//                        LOGGER("lockiiing")
                        obtainChainLock(event.getChainLockId(), event.getInstance(), true);
//                        LOGGER("locked")
                        event.setEventId(generatedChainId);
                        return event;
                    };
            return fireRawChainAction(f, generatedActionId, generatedChainId);
        }

        auto unlockChain() {

            auto generatedActionId = generateActionId<unsigned long>();
            auto generatedChainId = nextChainId();
            std::function<ChainEvent<EventType>(ChainEvent<EventType>)> f =
                    [generatedChainId](ChainEvent<EventType> event, auto... args) -> ChainEvent<EventType> {
                        obtainChainLock(event.getChainLockId(), event.getInstance(), false);
                        releaseChainLock(event.getBaseChainId(), event.getInstance());
                        event.setEventId(generatedChainId);
                        return event;
                    };
            return fireRawChainAction(f, generatedActionId, generatedChainId);
        }

        //@todo func or std::function<RetType(EventType,Args...)> ?
        template<typename Func, typename ... SetArgs>
        auto fireNewChainAction(Func func, SetArgs... setArgs) {
            using RetType = std::invoke_result_t<Func, EventType, SetArgs...>;
            auto generatedActionId = generateActionId<unsigned long>();
            auto generatedChainId = nextChainId();
            if constexpr (!std::is_same<void, RetType>::value) { //Return Type of func is not void
                std::function<ChainEvent<RetType>(ChainEvent<EventType>)> f =
                        [=, chainId = chainId](ChainEvent<EventType> chainedEvent) {
                            LOGGER("chain action " + (chainId ? *chainId : "NO CHAIN ID") + " stage : " +
                                   chainedEvent.getEventId())
                            obtainChainLock(chainedEvent.getChainLockId(), chainedEvent.getInstance(), false);
                            if (chainedEvent.getActualEvent() && chainedEvent.getActualEvent()->isEventValid()) {
                                LOGGER("chained action" + *chainId)
                                auto value = func(*chainedEvent.getActualEvent(), setArgs...);
                                ChainEvent<RetType> result(chainedEvent.getBaseChainId(), generatedChainId,
                                                           chainedEvent.getInstance(), value,
                                                           chainedEvent.getChainLockId());
//                            LOGGER("storing chain result in chain context " + std::to_string(Context::getActiveContext()->get<ChainContext>().use_count()) )
                                Context::getActiveContext()->get<ChainContext>()->storeChainResult<RetType>(
                                        generatedChainId, value);
                                return result;
                            } else {
                                return ChainEvent<RetType>(); //it has empty actual event and should stop the chain.
                            }

                        };
                return fireRawChainAction(f, generatedActionId, generatedChainId, setArgs...);
            } else { //Return Type of func is void

                std::function<void(ChainEvent<EventType>)> f = [=, chainId = chainId](
                        ChainEvent<EventType> chainedEvent) {
                    LOGGER("chain action " + (chainId ? *chainId : "NO CHAIN ID") + " stage : " +
                           chainedEvent.getEventId())

                    if (chainedEvent.getActualEvent()) {
                        func(*chainedEvent.getActualEvent(), setArgs...);
                    }

                };

                return fireRawChainAction(f, generatedActionId, generatedChainId, setArgs...);
            }


        }


    protected:
        //@todo func or std::function<RetType(EventType,Args...)> ?
        template<typename RetType, typename ... SetArgs>
        auto
        fireRawChainAction(std::function<ChainEvent<RetType>(ChainEvent<EventType>)> f, unsigned long generatedActionId,
                           const ChainIdType &generatedChainId, SetArgs... setArgs) {
            if (chainId) {
                //@todo apply constraint?
                if constexpr (!std::is_same<void, RetType>::value) { //Return Type of func is not void
                    LOGGER("generated chain id " + generatedChainId);
                    logicManager.setActionExtended<ChainEvent<RetType>, ChainEvent<EventType>, Args...>(
                            generatedActionId, f);
                    logicManager.assignAction<ChainEvent<EventType>, Args...>(*chainId, generatedActionId);
                    //@todo return LogicChainHelper for <RetType>, but what about Args... ?
                    LogicChainHelper<RetType> retLogicHelper(EventHelper<RetType>(), logicManager, generatedChainId);
                    return retLogicHelper;
                } else { //Return Type of func is void
                    logicManager.setAction<ChainEvent<EventType>, Args...>(generatedActionId, f);
                    logicManager.assignAction<ChainEvent<EventType>, Args...>(*chainId, generatedActionId);
                    return *this;
                }

            } else {

                //@todo exception!!!
                LOGGER("FAILURE! NO EVENT CHAIN ID SET!")
                //@todo generated chain if no chain set! fix!
//                auto generatedChainId = nextChainId();
                if constexpr (!std::is_same<void, RetType>::value) {
                    LogicChainHelper<RetType> retLogicHelper(EventHelper<RetType>(), logicManager, generatedChainId);
                    return retLogicHelper;
                } else {
                    return *this;
                }
            }

        }
//
//        //@todo func or std::function<RetType(EventType,Args...)> ?
//        template<template<typename> typename container,typename RetType, typename ... SetArgs>
//        auto
//        fireRawChainActionPack(std::function<container<ChainEvent<RetType>>(ChainEvent<EventType>)> f, unsigned long generatedActionId,
//                           const ChainIdType &generatedChainId, SetArgs... setArgs) {
//            if (chainId) {
//                //@todo apply constraint?
//                if constexpr (!std::is_same<void, RetType>::value) { //Return Type of func is not void
//                    LOGGER("generated chain id " + generatedChainId);
//                    logicManager.setActionExtendedPack<container,ChainEvent<RetType>, ChainEvent<EventType>, Args...>(
//                            generatedActionId, f);
//                    logicManager.assignAction<ChainEvent<EventType>, Args...>(*chainId, generatedActionId);
//                    //@todo return LogicChainHelper for <RetType>, but what about Args... ?
//                    LogicChainHelper<RetType> retLogicHelper(EventHelper<RetType>(), logicManager, generatedChainId);
//                    return retLogicHelper;
//                } else { //Return Type of func is void
//                    logicManager.setAction<ChainEvent<EventType>, Args...>(generatedActionId, f);
//                    logicManager.assignAction<ChainEvent<EventType>, Args...>(*chainId, generatedActionId);
//                    return *this;
//                }
//
//            } else {
//
//                //@todo exception!!!
//                LOGGER("FAILURE! NO EVENT CHAIN ID SET!")
//                //@todo generated chain if no chain set! fix!
////                auto generatedChainId = nextChainId();
//                if constexpr (!std::is_same<void, RetType>::value) {
//                    LogicChainHelper<RetType> retLogicHelper(EventHelper<RetType>(), logicManager, generatedChainId);
//                    return retLogicHelper;
//                } else {
//                    return *this;
//                }
//            }
//
//        }

    protected:

        static ChainLockIdType getLockIdFromChainId(const ChainIdType& chainId) {
            return ":>" + chainId;
        }

        static ChainLockIdType getLockIdFromChainGroupId(const ChainIdType& chainId) {
            return "::" + chainId;
        }


    public:

        ThisType &newChain(const ChainIdType &id,
                           const std::optional<LockConfiguration<EventType >> &lockConfiguration = std::nullopt) {
            chainId = id;
            return transform<ChainEvent<EventType>>([=](auto event) {
                LOGGER("starting chain " + id)
                //@todo be careful - if new chain starts before previous one has finished, the chain context will be overwritten. !!!! --- but is it really? I think active context should belong to an event.
                auto chainContext = Context::getActiveContext()->set<ChainContext>(id);

                auto lockId = lockConfiguration ? lockConfiguration->getLockId(event) : id;

                ChainEvent r(id, id, chainContext->instanceGenerator().nextValue(), event, lockId);
                LOGGER("new chain: " + id + " lock id " + lockId + " storing initial result " + r.getEventId())
                chainContext->storeChainResult<EventType>(r.getEventId(), *r.getActualEvent());
                //debug stuff @todo remove
                auto activeContext = Context::getActiveContext();
                activeContext->setDebug_id(id);
                return r;
            });

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
            auto &autoSource = logicManager.requireSource<AutoSource>();
            fireNewAction([&](EventType e, Args...) {
                autoSource.template generateEvent<NewEventType>();
            });


            return *this;
        }

        template<typename NewEventType, typename ... NewArgs>
        ThisType &emit(NewEventType newEventType) {
            auto &autoSource = logicManager.requireSource<AutoSource>();
            fireNewAction([&, newEventType](EventType e, Args...) {
                autoSource.template generateEvent<NewEventType>(newEventType);
            });

            return *this;
        }

        template<typename NewEventType>
        ThisType &emit(EventHelper<NewEventType> eventHelper) {
            return emit(eventHelper.makeEvent());
        }


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


    ChainGroup chainGroup(ChainIdType groupId) {
        return ChainGroup(std::move(groupId));
    }

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

    virtual bool assignActions(AssignActionHelper &actionHelper) = 0;

    virtual bool setupSources(SetupSourceHelper &sourceHelper) = 0;

};


#endif //BASYCO_LOGICOBJECT_H
