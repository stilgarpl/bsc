//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//

#ifndef BSC_PROPERTYDATA_H
#define BSC_PROPERTYDATA_H

#include "PropertyDefinitions.h"
#include <yaml-cpp/yaml.h>

namespace bsc {
    class InvalidPropertyKeyException : public std::domain_error {
    public:
        InvalidPropertyKeyException(const std::string& arg);
    };

    class InvalidPropertyTypeException : public std::domain_error {
    public:
        InvalidPropertyTypeException(const std::string& arg);
    };

    class PropertyData {
    private:
        const YAML::Node node;

    public:
        PropertyData(const YAML::Node& node);

        template<typename ValueType, typename KeyType>
        auto get(const KeyType& key) {
            return node[key].template as<ValueType>();
        }

        template<typename ValueType>
        auto get(const PropertyIdSequence& sequence) {
            //            YAML Node* currentNode = &node;
            //            for (const auto& key : sequence) {
            //                if (currentNode[key]) {
            //                    currentNode = currentNode[key];
            //                } else {
            //                    throw InvalidPropertyKeyException("Invalid key: " + key);
            //                }
            //            }
            //            if (node.IsDefined() && node.IsScalar()) {
            //                return node.as<ValueType>();
            //            }
            throw InvalidPropertyTypeException("Invalid type for property");
        }
    };
}// namespace bsc

#endif