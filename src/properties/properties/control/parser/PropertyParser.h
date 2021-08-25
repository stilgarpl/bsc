//
// Created by Krzysztof Tulidowicz on 21.01.2021.
//

#ifndef BSC_PROPERTYPARSER_H
#define BSC_PROPERTYPARSER_H

#include "properties/control/PropertyNodeType.h"
#include <mutex>
#include <properties/PropertyDefinitions.h>
#include <properties/control/PropertyStackKeeper.h>
namespace bsc {
    class PropertyParser {
    protected:
        mutable std::recursive_mutex mutex;

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

        /** selects next entry in the sequence if it exists
         *  usage:
         *  while(nextEntry()){
         *  ...
         *  }
         *
         * @return true if the entry is valid
         */
        virtual void nextEntry() = 0;

        /**
         * returns the type of the node
         * @return type of the current node
         */
        virtual PropertyNodeType getNodeType() const = 0;

        /**
         * return string value of the current node - if that node is scalar, otherwise throws exception
         * @return value of the current node
         */
        virtual PropertyValueType getValue() const = 0;

        /**
         * returns current id sequence as set by selectNode
         * @return
         */
        virtual PropertyIdSequence getCurrentIdSequence() const = 0;

        /**
         * gets value of the node chosen by the sequence. does not alter the currently selected node
         * @param idSequence
         * @return value of the selected node
         */
        PropertyValueType getValue(const PropertyIdSequence& idSequence);

        PropertyNodeType getNodeType(const PropertyIdSequence& idSequence);

        /**
         * pushes current node to the stack
         */
        virtual void push() = 0;

        /**
         * restores node from the stack, exception if stack is empty
         */
        virtual void pop() = 0;

        /**
         * returns size of current node (for sequences, but it will returns sane value for scalars too)
         */
        virtual std::size_t size() = 0;

        virtual bool hasEntry() = 0;

        friend class PropertyController;
    };

    template<typename T>
    concept IsPropertyParser = std::is_base_of_v<PropertyParser, T>;
}// namespace bsc

#endif