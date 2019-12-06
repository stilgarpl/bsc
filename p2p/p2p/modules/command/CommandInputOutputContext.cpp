//
// Created by stilgar on 06.12.2019.
//

#include "CommandInputOutputContext.h"

std::ostream& CommandInputOutputContext::out() {
    return outputStream;
}

std::istream& CommandInputOutputContext::in() {
    return inputStream;
}

const std::stringstream& CommandInputOutputContext::getInputStream() const {
    return inputStream;
}

const std::stringstream& CommandInputOutputContext::getOutputStream() const {
    return outputStream;
}
