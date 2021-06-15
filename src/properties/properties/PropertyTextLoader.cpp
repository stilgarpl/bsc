//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//

#include "PropertyTextLoader.h"
#include "Property.h"
#include "PropertyContext.h"
#include <context/context/Context.h>
#include <properties/control/parser/YamlParser.h>

namespace bsc {

    PropertyTextLoader::PropertyTextLoader(const std::string& text) {
        //@todo add support for other parser types -> factory like in other todos
        Context::getActiveContext()->get<PropertyContext>()->setPropertyParser<YamlParser>(text);
    }

}// namespace bsc
