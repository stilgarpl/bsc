//
// Created by Krzysztof Tulidowicz on 21.01.2021.
//

#include "YamlParser.h"
#include <libfyaml.h>
#include <span>
#include <yaml-cpp/yaml.h>

namespace bsc {

    void YamlParser::resetNode() {
        std::lock_guard g(mutex);
        //@todo should node reset clear the stack or should clearing the stack be separate function?
        while (!idStack.empty()) {
            //@todo C++20 will have erase algorithm, perhaps, maybe
            idStack.pop();
        }
        current = std::make_unique<decltype(root)>(root);
    }
    void YamlParser::selectNode(const PropertyIdSequence& propertyId) {
        std::lock_guard g(mutex);
        if (getNodeType() == PropertyParserNodeType::sequence) {
            current = std::make_unique<decltype(root)>((*current)[sequenceCount]);
            // sequence count is reset when selecting deeper node. if you want to continue iterating though this sequence, do push() before
            // selecting inner node
            currentPath += "[" + std::to_string(sequenceCount) + "]";
            sequenceCount = 0;
        }
        for (const auto& id : propertyId) {
            //@todo add validation if current is map type, else throw
            current = std::make_unique<decltype(root)>((*current)[id]);
            currentPath += "/" + id;
        }
    }

    void YamlParser::nextEntry() {
        std::lock_guard g(mutex);
        sequenceCount++;
    }

    PropertyParserNodeType YamlParser::getNodeType() {
        std::lock_guard g(mutex);
        switch (current->Type()) {
            case YAML::NodeType::value ::Map:
                return PropertyParserNodeType::map;
            case YAML::NodeType::value::Scalar:
                return PropertyParserNodeType ::scalar;
            case YAML::NodeType::value::Sequence:
                return PropertyParserNodeType ::sequence;
            case YAML::NodeType::value::Null:
                return PropertyParserNodeType ::empty;
            case YAML::NodeType::value::Undefined:
            default:
                return PropertyParserNodeType ::invalid;
        }
    }
    PropertyValueType YamlParser::getValue() {
        std::lock_guard g(mutex);
        switch (getNodeType()) {

            case PropertyParserNodeType::invalid:
            case PropertyParserNodeType::empty:
            case PropertyParserNodeType::map:
                //@todo throw
                break;
            case PropertyParserNodeType::scalar:
                return current->as<std::string>();
                break;
            case PropertyParserNodeType::sequence:
                return (*current)[sequenceCount].as<std::string>();
                break;
        }
        return "ERROR";//@todo remove, add throws for invalid
    }
    void YamlParser::push() {
        std::lock_guard g(mutex);

        idStack.push({.current = std::make_unique<decltype(root)>(*current), .sequenceCount = sequenceCount, .currentPath = currentPath});
    }
    void YamlParser::pop() {
        std::lock_guard g(mutex);

        current       = std::move(idStack.top().current);
        sequenceCount = idStack.top().sequenceCount;
        currentPath   = idStack.top().currentPath;
        idStack.pop();
    }
    //    YamlParser::YamlParser(const fs::path& yamlFile) {
    //        fyd           = fy_document_build_from_file(nullptr, yamlFile.c_str());
    //        currentFyNode = fy_document_root(fyd);
    //    }
    YamlParser::YamlParser(const std::string& yamlText) {
        root = YAML::Load(yamlText);
        resetNode();
    }

    std::size_t YamlParser::size() {
        std::lock_guard g(mutex);
        return current->size();
    }

    bool YamlParser::hasEntry() {
        std::lock_guard g(mutex);
        return sequenceCount < current->size();
    }

}// namespace bsc
