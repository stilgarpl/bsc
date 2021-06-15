//
// Created by Krzysztof Tulidowicz on 28.01.2021.
//

#include "PropertyFileLoader.h"
#include "PropertyContext.h"
#include "PropertyExceptions.h"
#include <properties/control/parser/YamlParser.h>

namespace bsc {

    PropertyFileLoader::PropertyFileLoader(const fs::path& path) {
        //@todo add support for different parsers - maybe add factory, like in Controller todo
        Context::getActiveContext()->get<PropertyContext>()->setPropertyParser<YamlParser>(path);
    }

}// namespace bsc
