//
// Created by stilgar on 05.12.2019.
//

#include <iostream>
#include "InputOutputContext.h"
#include <core/context/Context.h>

std::ostream& StandardInputOutputContext::out() {
    return std::cout;
}

std::istream& StandardInputOutputContext::in() {
    return std::cin;
}

InputOutputContext& InputOutputContext::active() {
    //@todo assert active context != null and get<IOC> != null
    return *Context::getActiveContext()->get<InputOutputContext>();

}
