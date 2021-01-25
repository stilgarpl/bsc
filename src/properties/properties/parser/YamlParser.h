//
// Created by Krzysztof Tulidowicz on 21.01.2021.
//

#ifndef BSC_YAMLPARSER_H
#define BSC_YAMLPARSER_H

#include "PropertyParser.h"
#include <memory>
#include <stack>
#include <yaml-cpp/yaml.h>

namespace bsc {

    class YamlParser : public PropertyParser {
        struct StackEntry {
            std::unique_ptr<YAML::Node> current;
            std::size_t sequenceCount;
            std::string currentPath = {};
        };

    private:
        YAML::Node root;
        std::stack<StackEntry> idStack{};
        std::unique_ptr<YAML::Node> current{};
        std::size_t sequenceCount = 0;
        //@todo this path is stored for debug and exception purposes. maybe it could be optimized ?
        std::string currentPath = {};

    public:
        void resetNode() override;
        void selectNode(const PropertyIdSequence& propertyId) override;
        void nextEntry() override;
        bool hasEntry() override;
        PropertyParserNodeType getNodeType() override;
        PropertyValueType getValue() override;
        void push() override;
        void pop() override;
        std::size_t size() override;
        //        YamlParser(const fs::path& yamlFile);
        explicit YamlParser(const std::string& yamlText);
    };
}// namespace bsc

#endif