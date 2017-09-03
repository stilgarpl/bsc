//
// Created by stilgar on 29.08.17.
//

#ifndef BASYCO_LOGICCONTEXT_H
#define BASYCO_LOGICCONTEXT_H


#include "../../../logic/LogicManager.h"

class LogicContext {

private:
    LogicManager &logicManager;

public:
    LogicContext(LogicManager &logicManager);

    LogicManager &getLogicManager() const;
};


#endif //BASYCO_LOGICCONTEXT_H
