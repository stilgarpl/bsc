//
// Created by stilgar on 06.07.18.
//

#ifndef BSC_LOGICOBJECT_H
#define BSC_LOGICOBJECT_H

#include <utility>
#include "LogicManager.h"
#include "sources/AutoSource.h"
#include "events/LogicStateEvent.h"
#include "chain/ChainEvent.h"
#include "events/EventWrapper.h"
#include "events/Tick.h"
#include "chain/ChainContext.h"
#include "chain/GlobalChainContext.h"
#include "chain/ChainGroup.h"
#include "chain/LockConfiguration.h"
#include "LogicExceptions.h"


namespace bsc {
    //@todo maybe move this to another file
    class InvalidChainException : public std::domain_error {
    public:
        InvalidChainException(const std::string& arg);
    };

//@todo replace Args... with AdditionalEventArgs... or sth
    class LogicObject {
    protected:
        LogicManager& logicManager;

    public:
        explicit LogicObject(LogicManager& logicManager);

    public:

    class SetupActionHelper {
        LogicManager& logicManager;
    public:
        explicit SetupActionHelper(LogicManager& logicManager) : logicManager(logicManager) {}

    public:
        template<typename EventType, typename... Args, typename ActionIdType>
        void setAction(ActionIdType id,
                       ActionManager::ActionType<const typename std::decay<EventType>::type&, Args...> action) {
            logicManager.setAction<EventType, Args...>(id, action);
        }

        template<typename... Args, typename ActionIdType, typename RetType>
        void setActionExtended(ActionIdType id, std::function<RetType(Args...)> func) {
            logicManager.setActionExtended(id, func);
        }
    };

    class AssignActionHelper {
        LogicManager& logicManager;
    public:
        explicit AssignActionHelper(LogicManager& logicManager) : logicManager(logicManager) {}

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
        LogicManager& logicManager;
    public:
        explicit SetupSourceHelper(LogicManager& logicManager) : logicManager(logicManager) {}

    public:
        template<typename SourceType, typename... Args>
        SourceType& requireSource(Args... args) {
            return logicManager.requireSource<SourceType, Args...>(args...);
        }
    };

    template<template<typename, typename...> typename SelfType, typename EventType, typename ... Args>
    class LogicChainHelper;

    template<typename eventType, typename ... Args>
    class EventHelper {
    public:
        typedef eventType EventType;
        typedef EventHelper<eventType, Args...> ThisType;
        typedef typename EventType::IdType EventId;
        typedef std::function<bool(EventType&)> ConstraintFunc;

    private:
        std::optional<EventId> eventId;
        std::list<ConstraintFunc> _constraint;
    public:
        EventHelper<EventType, Args...>& withId(EventId id) {
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

        explicit EventHelper(const std::optional<EventId>& eventId) : eventId(eventId) {}


        ThisType& constraint(const ConstraintFunc& func) {
            _constraint.push_back(func);
//            LOGGER("adding constraint, size " + std::to_string(_constraint.size()))
            return *this;
        }

        template<template<typename, typename...> typename NewSelfType, typename NewEventType, typename ... NewArgs>
        friend
        class LogicChainHelper;
    };

    template<typename eventType, typename ... Args>
    class StateHelper : public EventHelper<eventType, Args...> {

    public:
        StateHelper<eventType, Args...>& entered() {
            this->constraint(
                    [](eventType& event) -> bool { return event.getMethod() == LogicStateMethod::entered; });
            return *this;
        }

        StateHelper<eventType, Args...>& left() {
            this->constraint(
                    [](eventType& event) -> bool { return event.getMethod() == LogicStateMethod::left; });
            return *this;
        }

        StateHelper() = default;

        explicit StateHelper(const std::optional<typename EventHelper<eventType, Args...>::EventId>& eventId)
                : EventHelper<eventType, Args...>(eventId) {}
    };


public:
    template<template<typename, typename...> typename SelfType, typename eventType, typename ... Args>
    class LogicChainHelper {
    public:
        using EventType = eventType;
        typedef typename EventType::IdType EventId;
        using ThisType = SelfType<eventType, Args...>;
        typedef typename EventHelper<EventType, Args...>::ConstraintFunc ConstraintFunc;
    private:
        LogicManager& logicManager;
        LogicObject& logicObject;
        std::optional<EventId> eventId;
        std::optional<ChainIdType> chainId = std::nullopt;
        int childId = 0;
        std::list<ConstraintFunc> _constraint;
    protected:
        void setChainId(const std::optional<ChainIdType>& chainId) {
            LogicChainHelper::chainId = chainId;
        }

    public:
        const std::optional<ChainIdType>& getChainId() const {
            return chainId;
        }

        //@todo maybe this should return mutex guard? so obtained lock can be maintained through whole execution of chain block?
        static void
        obtainChainLock(const std::optional<ChainLockIdType>& chainLockId, InstanceType instance, bool lock) {
            //@todo mutex synchronize this whole method? on mutex from chainLock?
            if (!chainLockId) {
                //no chain lock id!
                //@todo maybe rearrange function so it doesn't return here.
//                LOGGER("no lock id")
                return;
            }
            LOGGER("chain lock id " + *chainLockId)
            auto chainContext = Context::getActiveContext()->get<GlobalChainContext>();
            auto& chainLock = chainContext->getChainLock(*chainLockId);
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
                guard.unlock();
                chainLock.lock(instance);
                guard.lock();
            }
            LOGGER("LOCK OBTAINED " + *chainLockId)
        }

        static void releaseChainLock(const std::optional<ChainLockIdType>& chainLockId, InstanceType instance) {
            //@todo mutex synchronize this whole method? on mutex from chainLock?
            if (chainLockId) {
                auto chainContext = Context::getActiveContext()->get<GlobalChainContext>();
                auto& chainLock = chainContext->getChainLock(*chainLockId);
                LOGGER("RELEASE BEFORE " + *chainLockId)
                std::unique_lock<std::recursive_mutex> guard(chainLock.getMutex());
                //@todo check instance.
                while (chainLock.isLocked() && chainLock.getInstance() != instance) {
                    chainLock.waitForUnlock();
                }
                chainLock.unlock();
                LOGGER("RELEASE AFTER")
            } else {
                //@todo error handling
                LOGGER("UNLOCK CALLED WITH NO CHAIN ID")
            }
        }

    public:
        LogicChainHelper(const EventHelper<EventType, Args...>& eventHelper, LogicManager& l, LogicObject& lo)
                : logicManager(l), logicObject(lo) {
            eventId = eventHelper.eventId;
            _constraint = eventHelper._constraint;
        }

        LogicChainHelper(const EventHelper<EventType, Args...>& eventHelper, LogicManager& l, LogicObject& lo,
                         const ChainIdType& id)
                : LogicChainHelper(eventHelper, l, lo) {
            chainId = id;
        };

        template<typename NewThisType>
        LogicChainHelper(const EventHelper<EventType, Args...>& eventHelper, LogicManager& l, LogicObject& lo,
                         const NewThisType& self)
                : LogicChainHelper(eventHelper, l, lo) {

        }


        ThisType& constraint(const ConstraintFunc& func) {
            _constraint.push_back(func);
            return *static_cast<ThisType*>(this);
        }

        template<typename ActionId>
        ThisType& fireAction(ActionId actionId) {
            LOGGER("assigning action ... ");
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
                        for (auto& i : constraint) {
                            result &= ((i)(e));
                        }

                        if (result) {
                            (*action)(e, args...);
                        }
                    };
                    logicManager.setAction<const typename std::decay<EventType>::type&, Args...>(generatedActionId,
                                                                                                 checkAction);
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
                using namespace std::string_literals;
                if constexpr (std::is_convertible_v<ActionId, std::string>) {
                    throw LogicAssignmentException("Failed to assign action : "s + actionId);
                } else {
                    throw LogicAssignmentException(
                            "Failed to assign action!"s); //@todo maybe some info of actionId, even if actionId cannot be converted to string?
                }

            }
            return *static_cast<ThisType*>(this);
        }

        //@todo this should be separate generator and should generate one specific type that is different so no one would make collisions.
        template<typename ActionId>
        auto generateActionId() {
            //@todo maybe create a version that will also work with strings? I have a lot of strings...
            static ActionId generatedActionId = 1;
            return generatedActionId++;
        }

        auto nextChildId() {
            //@todo replace with generator<>
            return childId++;
        }

        auto nextChainId() {
            //@todo replace with generator<>
            const std::string SEPARATOR = ".";
            if (!chainId.has_value()) {
                throw InvalidChainException("No chain id set");
            }
            return *chainId + SEPARATOR + std::to_string(nextChildId());
        }

        ThisType&
        fireNewAction(ActionManager::ActionType<const typename std::decay<EventType>::type&, Args...> newAction) {

            auto generatedActionId = generateActionId<unsigned long>();
//            int status;
//            auto realname = abi::__cxa_demangle(typeid(EventType).name(), 0, 0, &status);

//            LOGGER("fire new action for type " + std::string(typeid(EventType).name()) + " .... " + realname )
            LOGGER("assigning action with generated id " + std::to_string(generatedActionId));
            logicManager.setAction<const typename std::decay<EventType>::type&, Args...>(generatedActionId, newAction);
            fireAction(generatedActionId);
            return *static_cast<ThisType*>(this);
        }

        // template<typename ModuleType, typename RetType, typename ... Args>
        //        void mapCommand(const std::string &commandName, RetType (ModuleType::*f)(Args... args)) {
        //template<typename Type, typename RetType, typename ... MethodArgs>
        template<typename Type>
        ThisType& runMethod(Type* type, void(Type::*f)(const EventType&, Args...)) {
            /*placeholder is for event object*/
            return fireNewAction(std::bind(f, type, std::placeholders::_1));
        }

        template<typename Type>
        ThisType& runMethod(void(Type::*f)(const EventType&, Args...)) {
            /*placeholder is for event object*/
            return fireNewAction(std::bind(f, static_cast<Type*>(&logicObject), std::placeholders::_1));
        }

//        template<typename Type, typename ... MethodArgs>
//        ThisType& runGenericMethod(Type* type, void(Type::*f)(MethodArgs... args), MethodArgs... methodArgs){
//            return fireNewAction(std::bind_front(f,type,methodArgs...));
//        }

        template<typename Type, typename ... MethodArgs, typename ... Evaluators>
        auto runGenericMethod(Type* type, void(Type::*f)(MethodArgs...), Evaluators... evaluators) {

            return fireNewAction([=](const typename std::decay<EventType>::type& e) {
                (type->*f)(evaluators(e)...);
            });
        }

        template<typename Type, typename ... MethodArgs, typename ... Evaluators>
        auto runGenericMethod(void(Type::*f)(MethodArgs...), Evaluators... evaluators) {

            return fireNewAction([=, this](const typename std::decay<EventType>::type& e) {
                (static_cast<Type*>(&logicObject)->*f)(evaluators(e)...);
            });
        }

        template<typename GenericFunc, typename ... Evaluators>
        auto fireNewGenericAction(GenericFunc genericFunc, Evaluators... evaluators) {
            LOGGER("fireNewGenericAction - normal")
            return fireNewAction([=](const typename std::decay<EventType>::type& e) {
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
//                //@todo should I setDirect eventWrapper ID to something? I don't think it's ever fired as a standalone event, not wrapped in chainEvent...
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


        auto lockChain(std::function<ChainLockIdType(const ChainIdType&,
                                                     const typename std::decay<EventType>::type&)> lockIdGenerator = LockConfiguration::chain()) {
            auto generatedActionId = generateActionId<unsigned long>();
            auto generatedChainId = nextChainId();

            std::function<ChainEvent<typename std::decay<EventType>::type>(
                    const ChainEvent<typename std::decay<EventType>::type>&)> f =
                    [generatedChainId, lockIdGenerator](const ChainEvent<typename std::decay<EventType>::type>& event,
                                                        auto... args) -> ChainEvent<typename std::decay<EventType>::type> {
                        ChainEvent<typename std::decay<EventType>::type> newEvent(event);
                        auto generatedLockId = lockIdGenerator(event.getBaseChainId(), *event.getActualEvent());
//                        LOGGER("lockiiing")
                        obtainChainLock(generatedLockId, event.getInstance(), true);
//                        LOGGER("locked")
                        newEvent.setEventId(generatedChainId);
                        newEvent.setChainLockId(generatedLockId);
                        return newEvent;
                    };
            return fireRawChainAction(f, generatedActionId, generatedChainId);
        }

        auto unlockChain() {

            auto generatedActionId = generateActionId<unsigned long>();
            auto generatedChainId = nextChainId();
            std::function<ChainEvent<EventType>(const ChainEvent<EventType>&)> f =
                    [generatedChainId](ChainEvent<EventType> event, auto... args) -> ChainEvent<EventType> {
                        obtainChainLock(event.getChainLockId(), event.getInstance(), false);
                        releaseChainLock(event.getChainLockId(), event.getInstance());
                        event.setEventId(generatedChainId);
                        event.setChainLockId(std::nullopt);
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
                std::function<ChainEvent<RetType>(const ChainEvent<EventType>&)> f =
                        [=, chainId = chainId](ChainEvent<EventType> chainedEvent) {
//                            LOGGER("chain action " + (chainId ? *chainId : "NO CHAIN ID") + " stage : " +
//                                   chainedEvent.getEventId())
                            obtainChainLock(chainedEvent.getChainLockId(), chainedEvent.getInstance(), false);
                            if (chainedEvent.getActualEvent() && chainedEvent.getActualEvent()->isEventValid()) {
//                                LOGGER("chained action" + *chainId)
                                auto value = func(*chainedEvent.getActualEvent(), setArgs...);
                                ChainEvent<RetType> result(chainedEvent.getBaseChainId(), generatedChainId,
                                                           chainedEvent.getInstance(), value,
                                                           chainedEvent.getChainLockId());
//                            LOGGER("storing chain result in chain context " + std::to_string(Context::getActiveContext()->get<ChainContext>()->use_count()) )
                                Context::getActiveContext()->get<ChainContext>()->storeChainResult<RetType>(
                                        generatedChainId, value);
                                return result;
                            } else {
                                return ChainEvent<RetType>(); //it has empty actual event and should stop the chain.
                            }

                        };
                return fireRawChainAction(f, generatedActionId, generatedChainId, setArgs...);
            } else { //Return Type of func is void

                std::function<void(const ChainEvent<EventType>&)> f = [=, chainId = chainId](
                        const ChainEvent<EventType>& chainedEvent) {
//                    LOGGER("chain action " + (chainId ? *chainId : "NO CHAIN ID") + " stage : " +
//                           chainedEvent.getEventId())

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
        fireRawChainAction(std::function<ChainEvent<RetType>(const ChainEvent<EventType>&)> f,
                           unsigned long generatedActionId,
                           const ChainIdType& generatedChainId, SetArgs... setArgs) {
            if (chainId) {
                //@todo apply constraint?
                if constexpr (!std::is_same<void, RetType>::value) { //Return Type of func is not void
//                    LOGGER("generated chain id " + generatedChainId);
                    logicManager.setActionExtended<ChainEvent<RetType>, const ChainEvent<EventType>&, Args...>(
                            generatedActionId, f);
                    logicManager.assignAction<ChainEvent<EventType>, Args...>(*chainId, generatedActionId);
                    //@todo return LogicChainHelper for <RetType>, but what about Args... ?
                    SelfType<RetType> retLogicHelper(EventHelper<RetType>(), logicManager, logicObject,
                                                     *static_cast<ThisType*>(this));
                    retLogicHelper.setChainId(generatedChainId);
                    return retLogicHelper;
                } else { //Return Type of func is void
                    logicManager.setAction<const ChainEvent<EventType>&, Args...>(generatedActionId, f);
                    logicManager.assignAction<const ChainEvent<EventType>&, Args...>(*chainId, generatedActionId);
                    return *static_cast<ThisType*>(this);
                }

            } else {

                //@todo exception!!!
                LOGGER("FAILURE! NO EVENT CHAIN ID SET!")
                //@todo generated chain if no chain setDirect! fix!
//                auto generatedChainId = nextChainId();
                if constexpr (!std::is_same<void, RetType>::value) {
                    SelfType<RetType> retLogicHelper(EventHelper<RetType>(), logicManager, logicObject,
                                                     *static_cast<ThisType*>(this));
                    retLogicHelper.setChainId(generatedChainId);
                    return retLogicHelper;
                } else {
                    return *static_cast<ThisType*>(this);
                }
            }

        }

    public:

        //@todo this method could return a LogichChainHelper with a flag set (a bool or sth) - specialization would make it so that chain functions are only available if newChain was used previously and you can't use newChain after that - for example, LogicChainHelper can inherit from LogicChainHelperBase<...,bool isChain> - it would select base with chain function or without.
        ThisType& newChain(const ChainIdType& id) {
            chainId = id;
            return transform<ChainEvent<EventType>>([=](const auto& event) {
//                LOGGER("starting chain " + id)
                //@todo be careful - if new chain starts before previous one has finished, the chain context will be overwritten. !!!! --- but is it really? I think active context should belong to an event.
                auto chainContext = Context::getActiveContext()->set<ChainContext>(id);

                ChainEvent r(id, id, chainContext->instanceGenerator().nextValue(), event, std::nullopt);
//                LOGGER("new chain: " + id + " storing initial result " + r.getEventId())
                chainContext->storeChainResult<EventType>(r.getEventId(), *r.getActualEvent());
                //debug stuff @todo remove
                auto activeContext = Context::getActiveContext();
                activeContext->setDebugId(id);
                return r;
            });

        }

        template<typename NewEventType>
        ThisType& transform(std::function<NewEventType(const typename std::decay<EventType>::type&)> transformer) {
            //@todo maybe transform should return ThisType for new event type?
            auto generatedActionId = generateActionId<unsigned long>();
            std::function<NewEventType(const typename std::decay<EventType>::type&)> f = [=](
                    const typename std::decay<EventType>::type& event) -> NewEventType {
                return transformer(event);
            };
//            LOGGER("transforming action with generated id " + std::to_string(generatedActionId))
            logicManager.setActionExtended<NewEventType, EventType, Args...>(generatedActionId, f);
            fireAction(generatedActionId);
            return *static_cast<ThisType*>(this);
        }


        template<typename NewEventType, typename ... NewArgs>
        ThisType& emit() {
            auto& autoSource = logicManager.requireSource<AutoSource>();
            fireNewAction([&](EventType e, Args...) {
                autoSource.template generateEvent<NewEventType>();
            });


            return *static_cast<ThisType*>(this);
        }

        template<typename NewEventType, typename ... NewArgs>
        ThisType& emit(NewEventType newEventType) {
            auto& autoSource = logicManager.requireSource<AutoSource>();
            fireNewAction([&, newEventType](EventType e, Args...) {
                autoSource.template generateEvent<NewEventType>(newEventType);
            });

            return *static_cast<ThisType*>(this);
        }

        template<typename NewEventType>
        ThisType& emit(EventHelper<NewEventType> eventHelper) {
            return emit(eventHelper.makeEvent());
        }


        template<template<typename, typename...> typename NewSelfType, typename NewEventType, typename ... NewArgs>
        friend
        class LogicChainHelper;
    };

    template<template<typename, typename...> typename SelfType, typename EventType, typename ... Args>
    class SpecificLogicChainHelper : public LogicChainHelper<SelfType, EventType, Args...> {
    public:
        explicit SpecificLogicChainHelper(const EventHelper<EventType, Args...>& eventHelper, LogicManager& l,
                                          LogicObject& lo)
                : LogicChainHelper<SelfType, EventType, Args...>(eventHelper, l, lo) {};

        template<typename NewThisType>
        SpecificLogicChainHelper(const EventHelper<EventType, Args...>& eventHelper, LogicManager& l, LogicObject& lo,
                                 const NewThisType& self) : LogicChainHelper<SelfType, EventType, Args...>(eventHelper,
                                                                                                           l, lo,
                                                                                                           self) {

        }
    };

    template<template<typename, typename...> typename SelfType, typename Object, typename StateIdType, typename ... Args>
    class SpecificLogicChainHelper<SelfType, LogicStateEvent<Object, StateIdType>, Args...>
            : public LogicChainHelper<SelfType, LogicStateEvent<Object, StateIdType>, Args...> {
        typedef LogicStateEvent<Object, StateIdType> LogicEventType;
    public:
        explicit SpecificLogicChainHelper(const EventHelper<LogicEventType, Args...>& eventHelper, LogicManager& l,
                                          LogicObject& lo)
                : LogicChainHelper<SelfType, LogicEventType, Args...>(eventHelper, l, lo) {};

        template<typename NewThisType>
        SpecificLogicChainHelper(const EventHelper<LogicEventType, Args...>& eventHelper, LogicManager& l,
                                 LogicObject& lo,
                                 const NewThisType& self) : LogicChainHelper<SelfType, LogicEventType, Args...>(
                eventHelper, l, lo, self) {

        }

        typename LogicChainHelper<SelfType, LogicStateEvent<Object, StateIdType>, Args ...>::ThisType&
        fireStateChangeReaction(std::function<void(typename LogicEventType::Type&)> reaction) {
            return this->fireNewAction(
                    [reaction](auto event) {
                        reaction(event.getObject());
                    });
        }

        typename LogicChainHelper<SelfType, LogicStateEvent<Object, StateIdType>, Args ...>::ThisType&
        fireStateChangeReaction(std::function<void(typename LogicEventType::Type&, const StateIdType&)> reaction) {
            return this->fireNewAction(
                    [reaction](auto event) { reaction(event.getObject(), event.getState()); });
        }
    };

    template<typename EventType, typename ... Args>
    class SimpleLogicChainHelper : public SpecificLogicChainHelper<SimpleLogicChainHelper, EventType, Args...> {
    public:
        explicit SimpleLogicChainHelper(const EventHelper<EventType, Args...>& eventHelper, LogicManager& l,
                                        LogicObject& lo)
                : SpecificLogicChainHelper<SimpleLogicChainHelper, EventType, Args...>(eventHelper, l, lo) {};

        template<typename NewThisType>
        SimpleLogicChainHelper(const EventHelper<EventType, Args...>& eventHelper, LogicManager& l, LogicObject& lo,
                               const NewThisType& self)
                : SpecificLogicChainHelper<SimpleLogicChainHelper, EventType, Args...>(eventHelper,
                                                                                       l, lo, self) {

        }

    };


    static ChainGroup chainGroup(ChainIdType groupId) {
        return ChainGroup(std::move(groupId));
    }

    template<typename EventType, typename ... Args>
    static EventHelper<EventType, Args...> event() {
        return EventHelper<EventType, Args...>();
    }

    template<typename ObjectType, typename StateIdType = typename ObjectType::StateIdType, typename ... Args>
    static StateHelper<LogicStateEvent<ObjectType, StateIdType>, Args...> state() {
        return StateHelper<LogicStateEvent<ObjectType, StateIdType>, Args...>();
    }

    template<typename ObjectType, typename StateIdType = typename ObjectType::StateIdType, typename ... Args>
    static StateHelper<LogicStateEvent<ObjectType, StateIdType>, Args...> state(StateIdType id) {
        return StateHelper<LogicStateEvent<ObjectType, StateIdType>, Args...>(id);
    }

    template<typename EventType, typename ... Args>
    static EventHelper<EventType, Args...> event(typename EventType::IdType id) {
        return EventHelper<EventType, Args...>(id);
    }

protected:
    template<typename EventType, typename ... Args>
    SimpleLogicChainHelper<EventType, Args...> when(const EventHelper<EventType, Args...> eventHelper) {
        return SimpleLogicChainHelper<EventType, Args...>(eventHelper, logicManager, *this);
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

    virtual void setupActions(SetupActionHelper& actionHelper) = 0;

    virtual bool assignActions(AssignActionHelper& actionHelper) = 0;

    virtual bool setupSources(SetupSourceHelper& sourceHelper) = 0;

};
}


#endif //BSC_LOGICOBJECT_H
