//
// Created by Krzysztof Tulidowicz on 25.01.2021.
//

#include "PropertyParserController.h"
#include <context/context/Context.h>
#include <properties/PropertyContext.h>

namespace bsc {
    PropertyParserController::PropertyParserController(PropertyParser& parser) : propertyParser(parser), lockGuard(parser.mutex) {
        propertyParser.push();
    }
    PropertyParserController::PropertyParserController()
        : PropertyParserController(Context::getActiveContext()->get<PropertyContext>()->getPropertyParser()) {}
    PropertyParserController::~PropertyParserController() { propertyParser.pop(); }

}// namespace bsc
