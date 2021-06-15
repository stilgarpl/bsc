//
// Created by Krzysztof Tulidowicz on 25.01.2021.
//

#include "PropertyController.h"
#include <context/context/Context.h>
#include <properties/PropertyContext.h>

namespace bsc {
    PropertyController::PropertyController(PropertyParser& parser) : propertyParser(parser), lockGuard(parser.mutex) {
        propertyParser.push();
    }
    PropertyController::PropertyController()
        : PropertyController(Context::getActiveContext()->get<PropertyContext>()->getPropertyParser()) {}
    PropertyController::~PropertyController() { propertyParser.pop(); }



}// namespace bsc
