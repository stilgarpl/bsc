//
// Created by Krzysztof Tulidowicz on 28.01.2021.
//

#include "PropertyFileLoader.h"
#include "PropertyContext.h"
#include "PropertyEmptyLoader.h"
#include "PropertyExceptions.h"
#include <properties/control/parser/YamlParser.h>

namespace bsc {

    PropertyFileLoader::PropertyFileLoader(const fs::path& path, std::initializer_list<PropertySetting> settings) : PropertyLoader(settings){
        //@todo add support for different parsers - maybe add factory, like in Controller todo
        auto propertyContext = Context::getActiveContext()->get<PropertyContext>();
        try {
            propertyContext->setPropertyParser<YamlParser>(path);
        } catch (...) {
            //@todo this is just a hack, replace it with actual handling of missing files
            using namespace std::string_literals;
            propertyContext->setPropertyParser<YamlParser>(""s);
        }
    }

}// namespace bsc
