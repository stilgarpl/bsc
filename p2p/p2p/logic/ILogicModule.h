//
// Created by stilgar on 12.10.17.
//

#ifndef BASYCO_ILOGICMODULE_H
#define BASYCO_ILOGICMODULE_H


#include "LogicManager.h"

class ILogicModule {

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
            return logicManager.assignAction<EventType, Args..., ActionId>(eventId, actionId);
        }

        template<typename EventType, typename ActionId>
        bool assignAction(ActionId actionId) {
            return logicManager.assignAction<EventType, ActionId>(actionId);
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


public:
    virtual bool setupLogic(LogicManager &logicManager) {
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
