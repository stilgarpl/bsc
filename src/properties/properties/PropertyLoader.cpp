//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//

#include "PropertyLoader.h"
#include "PropertyContext.h"
#include <context/context/Context.h>
#include <yaml-cpp/yaml.h>

namespace bsc {
    std::shared_ptr<PropertyData> PropertyLoader::load(const fs::path& path) {
        return std::make_shared<PropertyData>(YAML::LoadFile(path));
    }
    std::shared_ptr<PropertyData> PropertyLoader::parse(const std::string& text) {
        return std::make_shared<PropertyData>(YAML::Load(text));
    }
    void PropertyLoader::loadToContext(const fs::path& path) {
        Context::getActiveContext()->get<PropertyContext>()->setPropertyData(load(path));
    }
    void PropertyLoader::parseToContext(const std::string& text) {
        Context::getActiveContext()->get<PropertyContext>()->setPropertyData(parse(text));
    }

}// namespace bsc
