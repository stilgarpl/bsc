//
// Created by stilgar on 05.12.2019.
//

#include "LogicExceptions.h"

LogicAssignmentException::LogicAssignmentException(const std::string& arg) : domain_error(arg) {}

LogicAssignmentException::LogicAssignmentException(const char* string) : domain_error(string) {}

LogicContextInvalid::LogicContextInvalid(const std::string& arg) : domain_error(arg) {}

LogicContextInvalid::LogicContextInvalid(const char* string) : domain_error(string) {}
