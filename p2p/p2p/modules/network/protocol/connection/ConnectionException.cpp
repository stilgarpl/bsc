//
// Created by stilgar on 09.12.2019.
//

#include "ConnectionException.h"

ConnectionException::ConnectionException(const std::string& arg) : domain_error(arg) {}
