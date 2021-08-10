//
// Created by Krzysztof Tulidowicz on 25.01.2021.
//

#ifndef BSC_PROPERTYCONTROLLER_H
#define BSC_PROPERTYCONTROLLER_H

#include "properties/control/parser/PropertyParser.h"
#include <properties/PropertyContext.h>
namespace bsc {
    class PropertyController {
        PropertyContext& propertyContext;
        const std::lock_guard<decltype(PropertyParser::mutex)> parserGuard;

    public:
        PropertyController();
        virtual ~PropertyController();
        auto& parser() { return propertyContext.getPropertyParser(); }
        auto& configuration() {return propertyContext.getPropertyConfiguration();}
    };


}// namespace bsc

#endif