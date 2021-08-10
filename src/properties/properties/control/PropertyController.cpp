//
// Created by Krzysztof Tulidowicz on 25.01.2021.
//

#include "PropertyController.h"
#include <context/context/Context.h>
#include <properties/PropertyContext.h>

namespace bsc {
    PropertyController::PropertyController()
        : propertyContext(Context::getActiveContext()->get<PropertyContext>()), parserGuard(propertyContext.getPropertyParser().mutex) {
        propertyContext.push();
    }
    PropertyController::~PropertyController() {
        propertyContext.pop();
    }

}// namespace bsc
