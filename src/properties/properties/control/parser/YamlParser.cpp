//
// Created by Krzysztof Tulidowicz on 21.01.2021.
//

#include "YamlParser.h"
#include "ParserExceptions.h"
#include <libfyaml.h>
#include <span>
#include <yaml-cpp/yaml.h>
#include <ranges>

namespace bsc {

    void YamlParser::resetNode() {
        std::lock_guard g(mutex);
        //@todo should node reset clear the stack or should clearing the stack be separate function?
        while (!idStack.empty()) {
            //@todo C++20 will have erase algorithm, perhaps, maybe
            idStack.pop();
        }
        current = std::make_unique<decltype(root)>(root);
        currentSequence = {};
        currentPath = {};
    }
    void YamlParser::selectNode(const PropertyIdSequence& propertyId) {
        std::lock_guard g(mutex);
        if (getNodeType() == PropertyNodeType::sequence) {
            current = std::make_unique<decltype(root)>((*current)[sequenceCount]);
            // sequence count is reset when selecting deeper node. if you want to continue iterating though this sequence, do push() before
            // selecting inner node
            currentPath += "[" + std::to_string(sequenceCount) + "]";
            sequenceCount = 0;
        }
        for (const auto& id : propertyId) {
            const auto& nodeType = getNodeType();
            if (nodeType == PropertyNodeType::map || nodeType == PropertyNodeType::sequence || nodeType == PropertyNodeType::empty) {
                current = std::make_unique<decltype(root)>((*current)[id]);
                currentPath += "/" + id;
            }
        }
        std::ranges::copy(propertyId,std::back_inserter(currentSequence));
    }

    void YamlParser::nextEntry() {
        std::lock_guard g(mutex);
        sequenceCount++;
    }

    PropertyNodeType YamlParser::getNodeType() const {
        std::lock_guard g(mutex);
        switch (current->Type()) {
            case YAML::NodeType::value ::Map:
                return PropertyNodeType::map;
            case YAML::NodeType::value::Scalar:
                return PropertyNodeType ::scalar;
            case YAML::NodeType::value::Sequence:
                return PropertyNodeType ::sequence;
            case YAML::NodeType::value::Null:
                return PropertyNodeType ::empty;
            case YAML::NodeType::value::Undefined:
            default:
                return PropertyNodeType ::invalid;
        }
    }
    PropertyValueType YamlParser::getValue() const {
        std::lock_guard g(mutex);
        switch (getNodeType()) {

            case PropertyNodeType::invalid:
            case PropertyNodeType::empty:
            case PropertyNodeType::map:
                //@todo throw
                break;
            case PropertyNodeType::scalar:
                return current->as<std::string>();
                break;
            case PropertyNodeType::sequence:
                return (*current)[sequenceCount].as<std::string>();
                break;
        }
        return "ERROR";//@todo remove, add throws for invalid
    }
    void YamlParser::push() {
        std::lock_guard g(mutex);
        idStack.push({.current = std::make_unique<decltype(root)>(*current), .sequenceCount = sequenceCount, .currentPath = currentPath, .currentSequence = currentSequence });
    }
    void YamlParser::pop() {
        std::lock_guard g(mutex);

        current       = std::move(idStack.top().current);
        sequenceCount = idStack.top().sequenceCount;
        currentPath   = idStack.top().currentPath;
        idStack.pop();
    }
    YamlParser::YamlParser(const fs::path& yamlFile) {
        root = YAML::LoadFile(yamlFile);
        YamlParser::resetNode();
    }
    YamlParser::YamlParser(const std::string& yamlText) {
        root = YAML::Load(yamlText);
        YamlParser::resetNode();
    }

    std::size_t YamlParser::size() {
        std::lock_guard g(mutex);
        return current->size();
    }

    bool YamlParser::hasEntry() {
        std::lock_guard g(mutex);
        return sequenceCount < current->size();
    }
    PropertyIdSequence YamlParser::getCurrentIdSequence() const {
        return bsc::PropertyIdSequence();
    }

}// namespace bsc
