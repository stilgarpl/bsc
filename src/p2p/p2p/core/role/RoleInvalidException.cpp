//
// Created by Krzysztof Tulidowicz on 12.11.17.
//

#include "RoleInvalidException.h"

bsc::RoleInvalidException::RoleInvalidException(const std::string& arg) : domain_error(arg) {}
