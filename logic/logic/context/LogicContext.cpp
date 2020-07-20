//
// Created by Krzysztof Tulidowicz on 29.08.17.
//

#include "LogicContext.h"


bsc::LogicContext::LogicContext(LogicManager& logicManager) : logicManager(logicManager) {}

bsc::LogicManager& bsc::LogicContext::getLogicManager() const {
    return logicManager;
}
