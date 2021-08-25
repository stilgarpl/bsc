//
// Created by Krzysztof Tulidowicz on 25.01.2021.
//

#ifndef BSC_PROPERTYCONTROLLER_H
#define BSC_PROPERTYCONTROLLER_H

#include "properties/control/parser/PropertyParser.h"
#include "properties/control/writer/PropertyWriter.h"
#include <properties/PropertyContext.h>
namespace bsc {
    class PropertyController final {
        PropertyContext& propertyContext;
        const std::scoped_lock<decltype(PropertyContext::mutex)> propertyGuard;
        PropertyStackKeeper<PropertyContext> contextKeeper;

    public:
        PropertyController();
        auto& parser() { return propertyContext.getPropertyParser(); }
        auto& configuration() {return propertyContext.getPropertyConfiguration();}
        auto& context() { return propertyContext;};
    };


}// namespace bsc

#endif