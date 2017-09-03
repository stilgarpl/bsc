//
// Created by stilgar on 29.08.17.
//

#include "LogicContext.h"

LogicContext::LogicContext(LogicManager &logicManager) : logicManager(logicManager) {}

LogicManager &LogicContext::getLogicManager() const {
    return logicManager;
}
