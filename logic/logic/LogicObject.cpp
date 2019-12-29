//
// Created by stilgar on 06.07.18.
//

#include "LogicObject.h"

LogicObject::LogicObject(LogicManager &logicManager) : logicManager(logicManager) {}


InvalidChainException::InvalidChainException(const std::string& arg) : domain_error(arg) {}
