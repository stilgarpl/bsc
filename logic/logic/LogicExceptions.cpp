//
// Created by stilgar on 05.12.2019.
//

#include "LogicExceptions.h"

bsc::LogicAssignmentException::LogicAssignmentException(const std::string& arg) : domain_error(arg) {}

bsc::LogicAssignmentException::LogicAssignmentException(const char* string) : domain_error(string) {}

bsc::LogicContextInvalid::LogicContextInvalid(const std::string& arg) : domain_error(arg) {}

bsc::LogicContextInvalid::LogicContextInvalid(const char* string) : domain_error(string) {}
