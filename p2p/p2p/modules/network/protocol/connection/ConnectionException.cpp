//
// Created by Krzysztof Tulidowicz on 09.12.2019.
//

#include "ConnectionException.h"

bsc::ConnectionException::ConnectionException(const std::string& arg) : domain_error(arg) {}
