//
// Created by Krzysztof Tulidowicz on 10.08.2021.
//
#include "PropertyEmptyLoader.h"
#include "PropertyContext.h"
#include <properties/control/parser/YamlParser.h>

namespace bsc {
    PropertyEmptyLoader::PropertyEmptyLoader() : PropertyLoader({}) {
        using namespace std::string_literals;
        Context::getActiveContext()->get<PropertyContext>()->setPropertyParser<YamlParser>(""s);
    }
}
