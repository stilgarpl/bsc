//
// Created by stilgar on 06.07.18.
//

#ifndef BASYCO_LOGICOBJECT_H
#define BASYCO_LOGICOBJECT_H


#include "LogicManager.h"

#include <p2p/logic/sources/AutoSource.h>

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
        typedef typename EventType::IdType EventId;

    private:
        std::optional<EventId> eventId;
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

        EventHelper() {}

        EventHelper(const std::optional<EventId> &eventId) : eventId(eventId) {}

        friend class LogicChainHelper<EventType, Args...>;
    };

    template<typename EventType, typename ... Args>
    class LogicChainHelper {
    public:
        typedef typename EventType::IdType EventId;
        typedef LogicChainHelper<EventType, Args...> ThisType;
    private:
        std::optional<EventId> eventId;
        LogicManager &logicManager;
    public:
        explicit LogicChainHelper(const EventHelper<EventType, Args...> &eventHelper, LogicManager &l) : logicManager(
                l) {
            eventId = eventHelper.eventId;
        }

        template<typename ActionId>
        ThisType &fireAction(ActionId actionId) {
            std::cout << "assigning action ... " << std::endl;
            bool success;
            //@todo throw some exception or sth if action isn't found
            if (eventId) {

                success = logicManager.assignAction<EventType, Args...>(*eventId, actionId);
            } else {

                success = logicManager.assignAction<EventType, Args...>(actionId);
            }
            if (!success) {
                throw std::string("Failed to assign action!");
            }
            return *this;
        }

        //@todo figure out how to get module pointer from this place
//        template<typename RetType,typename ModuleType,typename... ModArgs>
//        ThisType& fireModuleAction(RetType (ModuleType::*f)(ModArgs... args)) {
//
//        }


        ThisType &fireNewAction(ActionManager::ActionType<EventType, Args...> action) {

            static unsigned long generatedActionId = 1;
            std::cout << "assigning action with generated id " << std::to_string(generatedActionId) << std::endl;
            logicManager.setAction<EventType, Args...>(generatedActionId, action);
            fireAction(generatedActionId);
            generatedActionId++;
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
