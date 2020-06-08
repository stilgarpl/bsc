//
// Created by stilgar on 08.06.2020.
//

#include "FileSortingExceptions.h"

namespace bsc {

    FileSortingException::FileSortingException(const std::string& arg) : domain_error(arg) {}

}// namespace bsc
