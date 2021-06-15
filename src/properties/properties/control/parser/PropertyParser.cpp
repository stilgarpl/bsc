//
// Created by Krzysztof Tulidowicz on 21.01.2021.
//

#include "PropertyParser.h"

namespace bsc {
    using StackKeeper = PropertyStackKeeper<PropertyParser>;
    PropertyValueType PropertyParser::getValue(const PropertyIdSequence& idSequence) {
        StackKeeper keeper(*this);// this will restore node to state before select
        selectNode(idSequence);
        return getValue();
    }

    PropertyParserNodeType PropertyParser::getNodeType(const PropertyIdSequence& idSequence) {
        StackKeeper keeper(*this);// this will restore node to state before select
        selectNode(idSequence);
        return getNodeType();
    }
}// namespace bsc
