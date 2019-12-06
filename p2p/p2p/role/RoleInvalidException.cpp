//
// Created by stilgar on 12.11.17.
//

#include "RoleInvalidException.h"

RoleInvalidException::RoleInvalidException(const std::string& arg) : domain_error(arg) {}
