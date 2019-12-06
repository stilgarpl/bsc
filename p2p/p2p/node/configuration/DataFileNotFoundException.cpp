//
// Created by stilgar on 31.05.19.
//

#include "DataFileNotFoundException.h"

DataFileNotFoundException::DataFileNotFoundException(const std::string& arg) : domain_error(arg) {}
