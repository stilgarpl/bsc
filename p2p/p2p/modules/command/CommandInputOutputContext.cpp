//
// Created by stilgar on 06.12.2019.
//

#include "CommandInputOutputContext.h"

std::ostream& bsc::CommandInputOutputContext::out() {
    return outputStream;
}

std::istream& bsc::CommandInputOutputContext::in() {
    return inputStream;
}

const std::stringstream& bsc::CommandInputOutputContext::getInputStream() const {
    return inputStream;
}

const std::stringstream& bsc::CommandInputOutputContext::getOutputStream() const {
    return outputStream;
}

std::ostream& bsc::CommandInputOutputContext::err() {
    return errorStream;
}

const std::stringstream& bsc::CommandInputOutputContext::getErrorStream() const {
    return errorStream;
}
