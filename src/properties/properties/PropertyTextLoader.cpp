//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//

#include "PropertyTextLoader.h"
#include "Property.h"
#include "PropertyContext.h"
#include "PropertyExceptions.h"
#include <context/context/Context.h>
#include <properties/parser/YamlParser.h>

namespace bsc {

    PropertyTextLoader::PropertyTextLoader(const std::string& text) {
        Context::getActiveContext()->get<PropertyContext>()->setPropertyParser<YamlParser>(text);
    }

}// namespace bsc
