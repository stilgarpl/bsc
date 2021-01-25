//
// Created by Krzysztof Tulidowicz on 25.01.2021.
//

#ifndef BSC_PROPERTYPREFIX_H
#define BSC_PROPERTYPREFIX_H

#include "PropertyContext.h"
#include "PropertyDefinitions.h"
#include <parser/parser/explode.h>

namespace bsc {

    template<detail::StringLiteral lit>
    class PropertyPrefix {
        static inline const PropertyIdType propertyId     = {lit.value};
        static inline const PropertyIdSequence idSequence = explode(propertyId, propertyDelimiter);

    public:
        PropertyPrefix() { Context::getActiveContext()->get<PropertyContext>()->getPropertyParser().selectNode(idSequence); }
    };
}// namespace bsc

#endif