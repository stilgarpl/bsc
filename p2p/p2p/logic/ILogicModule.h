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
        void operator()(ActionIdType id, ActionManager::ActionType<Args...> action) {
            logicManager.setAction<Args...>(id, action);
        };
    };


public:
    virtual bool setupLogic(LogicManager &logicManager) {
        bool ret = true;
        ret &= setupSources(logicManager);
        SetupActionHelper actionHelper(logicManager);
        setupActions(logicManager);
//        setupActions(actionHelper);
        ret &= assignActions(logicManager);
        return ret;
    };

    virtual void setupActions(LogicManager &logicManager) = 0;
//    virtual void setupActions(SetupActionHelper& actionHelper) = 0;

    virtual bool assignActions(LogicManager &logicManager)=0;

    virtual bool setupSources(LogicManager &logicManager)=0;
};


#endif //BASYCO_ILOGICMODULE_H
