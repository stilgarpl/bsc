//
// Created by stilgar on 05.12.2019.
//

#include <iostream>
#include "InputOutputContext.h"

#include <core/context/Context.h>

std::ostream& bsc::StandardInputOutputContext::out() {
    return std::cout;
}

std::istream& bsc::StandardInputOutputContext::in() {
    return std::cin;
}

std::ostream& bsc::StandardInputOutputContext::err() {
    return std::cerr;
}

bsc::InputOutputContext& bsc::InputOutputContext::active() {
    return bsc::Context::getActiveContext()->get<bsc::InputOutputContext>();

}
