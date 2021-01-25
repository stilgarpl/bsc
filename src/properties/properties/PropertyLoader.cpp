//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//

#include "PropertyLoader.h"
#include "PropertyContext.h"
#include <context/context/Context.h>
#include <properties/parser/YamlParser.h>
#include <yaml-cpp/yaml.h>

namespace bsc {

    //    void PropertyLoader::loadToContext(const fs::path& path) {
    //        Context::getActiveContext()->get<PropertyContext>()->setPropertyParser<YamlParser>();
    //    }
    void PropertyLoader::parseToContext(const std::string& text) {
        Context::getActiveContext()->get<PropertyContext>()->setPropertyParser<YamlParser>(text);
    }

}// namespace bsc
