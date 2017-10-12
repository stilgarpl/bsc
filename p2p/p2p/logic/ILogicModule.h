//
// Created by stilgar on 12.10.17.
//

#ifndef BASYCO_ILOGICMODULE_H
#define BASYCO_ILOGICMODULE_H


#include "LogicManager.h"

class ILogicModule {

public:
    virtual bool setupLogic(LogicManager &logicManager) {
        bool ret = true;
        ret &= setupSources(logicManager);
        setupActions(logicManager);
        ret &= assignActions(logicManager);
        return ret;
    };

    virtual void setupActions(LogicManager &logicManager) = 0;

    virtual bool assignActions(LogicManager &logicManager)=0;

    virtual bool setupSources(LogicManager &logicManager)=0;
};


#endif //BASYCO_ILOGICMODULE_H
