//
// Created by stilgar on 12.10.17.
//

#ifndef BASYCO_ILOGICMODULE_H
#define BASYCO_ILOGICMODULE_H


#include "LogicManager.h"

class ILogicModule {

public:
    virtual bool setupLogic(LogicManager &logicManager) =0;
};


#endif //BASYCO_ILOGICMODULE_H
