//
// Created by Krzysztof Tulidowicz on 21.01.2021.
//

#include "PropertyParser.h"

namespace bsc {
    PropertyParser::StackKeeper::StackKeeper(PropertyParser& propertyParser) : propertyParser(propertyParser) { propertyParser.push(); }
    PropertyParser::StackKeeper::~StackKeeper() { propertyParser.pop(); }
}// namespace bsc
