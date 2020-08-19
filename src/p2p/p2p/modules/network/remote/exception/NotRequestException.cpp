//
// Created by Krzysztof Tulidowicz on 09.07.18.
//

#include "NotRequestException.h"


bsc::NotRequestException::NotRequestException(const std::string& arg) : invalid_argument(arg) {}
