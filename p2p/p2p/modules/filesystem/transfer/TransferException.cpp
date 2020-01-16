//
// Created by stilgar on 19.06.19.
//

#include "TransferException.h"

bsc::TransferException::TransferException(const std::string& arg) : domain_error(arg) {}
