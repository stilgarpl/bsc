//
// Created by stilgar on 12.10.17.
//

#ifndef BASYCO_ILOGICMODULE_H
#define BASYCO_ILOGICMODULE_H


#include <p2p/logic/sources/AutoSource.h>
#include "LogicManager.h"

class ILogicModule {
protected:
    LogicManager &logicManager;
public:
    explicit ILogicModule(LogicManager &logicManager) : logicManager(logicManager) {}

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
            if (eventId) {

                logicManager.assignAction<EventType, Args...>(*eventId, actionId);
            } else {

                logicManager.assignAction<EventType, Args...>(actionId);
            }
            return *this;
        }


        ///@todo maybe add Args.. for actions with many arguments... maybe to whole helper class.
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
            ///@todo initialize some actual values.
//            logicManager.event<NewEventType,NewArgs...>(NewEventType(),NewArgs()...);
            auto &autoSource = logicManager.requireSource<AutoSource>();
            fireNewAction([&](EventType e, Args...) {
                autoSource.generateEvent<NewEventType>();
            });


            return *this;
        };

        template<typename NewEventType, typename ... NewArgs>
        ThisType &emit(NewEventType newEventType) {
            ///@todo initialize some actual values.
//            logicManager.event<NewEventType,NewArgs...>(NewEventType(),NewArgs()...);
            auto &autoSource = logicManager.requireSource<AutoSource>();
            fireNewAction([&, newEventType](EventType e, Args...) {
                autoSource.generateEvent<NewEventType>(newEventType);
            });

            return *this;
        };

        template<typename NewEventType>
        ThisType &emit(EventHelper<NewEventType> eventHelper) {
            emit(eventHelper.makeEvent());
        }

    };


    template<typename EventType, typename ... Args>
    EventHelper<EventType, Args...> event() {
        return EventHelper<EventType, Args...>();
    }

    template<typename EventType, typename ... Args>
    LogicChainHelper<EventType, Args...> when(const EventHelper<EventType, Args...> eventHelper) {
        return LogicChainHelper<EventType, Args...>(eventHelper, logicManager);
    }


protected:

    //when(event<CommandEvent>().withId(CommandId::EXECUTE_COMMAND)).fireNewAction(RUN_COMMAND);d

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
//    virtual void setupActions(SetupActionHelper& actionHelper) = 0;
//    virtual bool assignActions(ILogicModule::AssignActionHelper &actionHelper)=0;
    virtual bool assignActions(AssignActionHelper &actionHelper)=0;

    virtual bool setupSources(SetupSourceHelper &sourceHelper)=0;


};


#endif //BASYCO_ILOGICMODULE_H
