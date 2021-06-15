//
// Created by stilgar on 14.06.2021.
//

#ifndef BSC_PROPERTYWRITER_H
#define BSC_PROPERTYWRITER_H

#include <mutex>
#include <properties/PropertyDefinitions.h>
#include <properties/control/parser/PropertyParserNodeType.h>
namespace bsc {
    class PropertyWriter {
    protected:
        std::recursive_mutex mutex;
    public:
        /**
         * resets selected node to root node
         */
        virtual void resetNode() = 0;

        /**
         * sets selected node to property id - as prefix - relative to currently selected node
         * @param propertyId
         */
        virtual void selectNode(const PropertyIdSequence& propertyId) = 0;

        /**
       * pushes current node to the stack
       */
        virtual void push() = 0;

        /**
         * restores node from the stack, exception if stack is empty
         */
        virtual void pop() = 0;

        /**
         * set the value of the currently selected node to value
         * @param value - new value
         */
        virtual void setValue(const PropertyValueType& value) =0;
        virtual void nextEntry() = 0;
        virtual PropertyParserNodeType getNodeType() =0;
    };

    template<typename T>
    concept IsPropertyWriter = std::is_base_of_v<PropertyWriter, T>;
    template<typename T>
    concept IsWritablePropertyClass = requires(T t, PropertyWriter& propertyWriter) {
        t.write(propertyWriter);
    };
}// namespace bsc
#endif// BSC_PROPERTYWRITER_H
