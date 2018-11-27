//
// Created by stilgar on 06.07.18.
//

#ifndef BASYCO_LOGICOBJECT_H
#define BASYCO_LOGICOBJECT_H


#include "LogicManager.h"

#include <p2p/logic/sources/AutoSource.h>
#include <p2p/logic/events/LogicStateEvent.h>

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
    private:
        class ConstrainedActionEvent : public IEvent<unsigned long> {

        };
    public:
        typedef typename EventType::IdType EventId;
        typedef LogicChainHelper<EventType, Args...> ThisType;
        typedef typename EventHelper<EventType, Args...>::ConstraintFunc ConstraintFunc;
    private:
        std::optional<EventId> eventId;

        std::list<ConstraintFunc> _constraint;
        LogicManager &logicManager;
    public:
        explicit LogicChainHelper(const EventHelper<EventType, Args...> &eventHelper, LogicManager &l) : logicManager(
                l) {
            eventId = eventHelper.eventId;
            _constraint = eventHelper._constraint;
//            LOGGER("constraint ss " + std::to_string(_constraint.size()))
        }

        //@todo add multiple constraints
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

        ThisType &fireNewAction(ActionManager::ActionType<EventType, Args...> action) {

            auto generatedActionId = generateActionId<unsigned long>();

            std::cout << "assigning action with generated id " << std::to_string(generatedActionId) << std::endl;
//            if (_constraint) {
//                //action is constrained, wrap it in check action
//                auto checkAction = [=,constraint=_constraint](EventType e, Args... args){
//                    if ((*constraint)(e)) {
//                        action(e, args...);
//                    }
//                };
//                logicManager.setAction<EventType, Args...>(generatedActionId, checkAction);
//            } else {
            logicManager.setAction<EventType, Args...>(generatedActionId, action);
//            }
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
    EventHelper<EventType, Args...> event() {
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
    EventHelper<EventType, Args...> event(typename EventType::IdType id) {
        return EventHelper<EventType, Args...>(id);
    }

    template<typename EventType, typename ... Args>
    LogicChainHelper<EventType, Args...> when(const EventHelper<EventType, Args...> eventHelper) {
        return LogicChainHelper<EventType, Args...>(eventHelper, logicManager);
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
