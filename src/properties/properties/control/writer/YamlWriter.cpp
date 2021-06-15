//
// Created by stilgar on 14.06.2021.
//

#include "YamlWriter.h"
#include <yaml-cpp/emitter.h>

namespace bsc {

    void YamlWriter::resetNode() {
        std::lock_guard g(mutex);
        //@todo should node reset clear the stack or should clearing the stack be separate function?
        while (!idStack.empty()) {
            //@todo C++20 will have erase algorithm, perhaps, maybe
            idStack.pop();
        }
        current = std::make_unique<decltype(root)>(root);
        root = *current;
    }
    void YamlWriter::selectNode(const PropertyIdSequence& propertyId) {
        std::lock_guard g(mutex);
        if (!isNodeNull()) {
        if (getNodeType() == PropertyParserNodeType::sequence) {
            current = std::make_unique<decltype(root)>((*current)[sequenceCount]);
            // sequence count is reset when selecting deeper node. if you want to continue iterating though this sequence, do push() before
            // selecting inner node
            currentPath += "[" + std::to_string(sequenceCount) + "]";
            sequenceCount = 0;
        }
        }
        for (const auto& id : propertyId) {
            //@todo add validation if current is map type, else throw
            auto newNode = std::make_unique<decltype(root)>();
            (*current)[id] = *newNode;
            current = std::move(newNode);
            currentPath += "/" + id;
        }
    }
    void YamlWriter::push() {
        std::lock_guard g(mutex);
        idStack.push({.current = std::make_unique<decltype(root)>(*current), .sequenceCount = sequenceCount, .currentPath = currentPath});
    }

    void YamlWriter::pop() {
        std::lock_guard g(mutex);

        current       = std::move(idStack.top().current);
        sequenceCount = idStack.top().sequenceCount;
        currentPath   = idStack.top().currentPath;
        idStack.pop();

    }
    void YamlWriter::setValue(const PropertyValueType& value) {
        std::lock_guard g(mutex);
        *current = value;
    }

    void YamlWriter::nextEntry() {
        std::lock_guard g(mutex);
        sequenceCount++;
    }

    PropertyParserNodeType YamlWriter::getNodeType() {
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
    std::string YamlWriter::writeToString() {
        YAML::Emitter out;
        out << root;
        return out.c_str();
    }
    bool YamlWriter::isNodeNull() {
        return *current;
    }
    YamlWriter::YamlWriter() {
        YamlWriter::resetNode();
    }
}// namespace bsc