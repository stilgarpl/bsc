//
// Created by Krzysztof Tulidowicz on 25.01.2021.
//

#include "PropertyController.h"
#include <context/context/Context.h>
#include <properties/PropertyContext.h>

namespace bsc {
    //@todo better locking of both writer and parser. maybe lock whole context instead?
    PropertyController::PropertyController()
        : propertyContext(Context::getActiveContext()->get<PropertyContext>()), propertyGuard(propertyContext.mutex), contextKeeper(propertyContext) {

    }


}// namespace bsc
