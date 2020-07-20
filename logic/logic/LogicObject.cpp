//
// Created by Krzysztof Tulidowicz on 06.07.18.
//

#include "LogicObject.h"


bsc::LogicObject::LogicObject(bsc::LogicManager& logicManager) : logicManager(logicManager) {}


bsc::InvalidChainException::InvalidChainException(const std::string& arg) : domain_error(arg) {}
