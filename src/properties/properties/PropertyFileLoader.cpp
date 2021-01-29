//
// Created by Krzysztof Tulidowicz on 28.01.2021.
//

#include "PropertyFileLoader.h"
#include "PropertyContext.h"
#include "PropertyExceptions.h"
#include <properties/parser/YamlParser.h>

namespace bsc {

    PropertyFileLoader::PropertyFileLoader(const fs::path& path) {
        Context::getActiveContext()->get<PropertyContext>()->setPropertyParser<YamlParser>(path);
    }

}// namespace bsc
