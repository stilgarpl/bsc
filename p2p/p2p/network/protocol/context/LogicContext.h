//
// Created by stilgar on 29.08.17.
//

#ifndef BASYCO_LOGICCONTEXT_H
#define BASYCO_LOGICCONTEXT_H


#include "p2p/logic/LogicManager.h"

class LogicContext {

private:
    LogicManager &logicManager;

public:
    LogicContext(LogicManager &logicManager);

    LogicManager &getLogicManager() const;

    //shortcut method for getting logic manager from active context
    static LogicManager &getLogicManagerFromActiveContext() {
        return Context::getActiveContext().get<LogicContext>()->getLogicManager();
    }
};


#endif //BASYCO_LOGICCONTEXT_H
