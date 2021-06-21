//
// Created by stilgar on 14.06.2021.
//

#ifndef BSC_YAMLWRITER_H
#define BSC_YAMLWRITER_H
#include "PropertyWriter.h"
#include <stack>
#include <yaml-cpp/yaml.h>
namespace bsc {
    class YamlWriter : public PropertyWriter {
    private:
        struct StackEntry {
            std::unique_ptr<YAML::Node> current;
            std::size_t sequenceCount;
            std::string currentPath = {};
            PropertyNodeType nodeType ;
        };

    private:
        YAML::Node root{};
        std::stack<StackEntry> idStack{};
        std::unique_ptr<YAML::Node> current{};
        std::size_t sequenceCount = 0;
        PropertyNodeType nodeType = PropertyNodeType::empty;
        //@todo this path is stored for debug and exception purposes. maybe it could be optimized ?
        std::string currentPath = {};
        bool isNodeNull();
    public:
        void resetNode() override;
        void selectNode(const PropertyIdSequence& propertyId) override;
        void push() override;
        void pop() override;
        void setValue(const PropertyValueType& value) override;
        void nextEntry() override;
        PropertyNodeType getNodeType() override;

        std::string writeToString();
        YamlWriter();
        void setNodeType(PropertyNodeType propertyNodeType) override;
    };
}
#endif// BSC_YAMLWRITER_H
