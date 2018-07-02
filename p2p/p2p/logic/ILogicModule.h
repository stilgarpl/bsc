//
// Created by stilgar on 12.10.17.
//

#ifndef BASYCO_ILOGICMODULE_H
#define BASYCO_ILOGICMODULE_H


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

    template<typename EventType>
    class LogicChainHelper;

    template<typename eventType>
    class EventHelper {
    public:
        typedef eventType EventType;
        typedef typename EventType::IdType EventId;

    private:
        std::optional<EventId> eventId;
    public:
        EventHelper<EventType> &withId(EventId id) {
            eventId = id;
            return *this;
        }

        friend class LogicChainHelper<eventType>;
    };

    template<typename EventType>
    class LogicChainHelper {
    public:
        typedef typename EventType::IdType EventId;
        typedef LogicChainHelper<EventType> ThisType;
    private:
        std::optional<EventId> eventId;
        LogicManager &logicManager;
    public:
        explicit LogicChainHelper(const EventHelper<EventType> &eventHelper, LogicManager &l) : logicManager(l) {
            eventId = eventHelper.eventId;
        }

        template<typename ActionId>
        ThisType &fireAction(ActionId actionId) {
            if (eventId) {
                logicManager.assignAction<EventType>(*eventId, actionId);
            } else {
                logicManager.assignAction<EventType>(actionId);
            }
            return *this;
        }
    };


    template<typename EventType>
    EventHelper<EventType> event() {
        return EventHelper<EventType>();
    }

    template<typename EventType>
    LogicChainHelper<EventType> when(const EventHelper<EventType> eventHelper) {
        return LogicChainHelper<EventType>(eventHelper, logicManager);
    }


protected:

    //when(event<CommandEvent>().withId(CommandId::EXECUTE_COMMAND)).fireAction(RUN_COMMAND);d

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
