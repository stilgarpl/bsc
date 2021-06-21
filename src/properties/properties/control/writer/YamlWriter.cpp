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
        root    = *current;
        nodeType = PropertyNodeType::empty;
    }
    void YamlWriter::selectNode(const PropertyIdSequence& propertyId) {
        std::lock_guard g(mutex);

            if (getNodeType() == PropertyNodeType::sequence) {
                current = std::make_unique<decltype(root)>((*current)[sequenceCount]);
                // sequence count is reset when selecting deeper node. if you want to continue iterating though this sequence, do push()
                // before selecting inner node
                currentPath += "[" + std::to_string(sequenceCount) + "]";
                sequenceCount = 0;
            }

        for (const auto& id : propertyId) {
            //@todo add validation if current is map type, else throw
            if ((*current)[id]) {
                current = std::make_unique<decltype(root)>((*current)[id]);
                currentPath += "/" + id;
            } else {
                auto newNode   = std::make_unique<decltype(root)>();
                (*current)[id] = *newNode;
                current        = std::move(newNode);
                currentPath += "/" + id;
            }
        }
        setNodeType(PropertyNodeType::empty); //@todo right value!!!!!!!!!!!!
    }
    void YamlWriter::push() {
        std::lock_guard g(mutex);
        idStack.push({.current       = std::make_unique<decltype(root)>(*current),
                      .sequenceCount = sequenceCount,
                      .currentPath   = currentPath,
                      .nodeType      = nodeType});
    }

    void YamlWriter::pop() {
        std::lock_guard g(mutex);
        auto&& top    = idStack.top();
        current       = std::move(top.current);
        sequenceCount = top.sequenceCount;
        currentPath   = top.currentPath;
        nodeType      = top.nodeType;
        idStack.pop();
    }
    void YamlWriter::setValue(const PropertyValueType& value) {
        std::lock_guard g(mutex);
        if (getNodeType() == PropertyNodeType::sequence) {
            (*current)[sequenceCount] = value;
        } else {
            *current = value;
        }
    }

    void YamlWriter::nextEntry() {
        std::lock_guard g(mutex);
        sequenceCount++;
    }

    PropertyNodeType YamlWriter::getNodeType() {
        std::lock_guard g(mutex);
        return nodeType;

    }
    std::string YamlWriter::writeToString() {
        std::lock_guard g(mutex);
        YAML::Emitter out;
        out << root;
        return out.c_str();
    }
    bool YamlWriter::isNodeNull() {
        std::lock_guard g(mutex);
        return *current;
    }
    YamlWriter::YamlWriter() {
        YamlWriter::resetNode();
    }
    void YamlWriter::setNodeType(PropertyNodeType propertyNodeType) {
        nodeType = propertyNodeType;
    }
}// namespace bsc