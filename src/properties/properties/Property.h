//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//

#ifndef BSC_PROPERTY_H
#define BSC_PROPERTY_H

#include "Property.h"
#include "PropertyContext.h"
#include "PropertyDefinitions.h"
#include "PropertyPrefix.h"
#include <algorithm>
#include <optional>
#include <parser/parser/explode.h>
#include <parser/parser/fromString.h>
#include <properties/parser/PropertyParserController.h>

namespace bsc {

    template<detail::StringLiteral lit, typename T = detail::DirectPropertyMapper>
    class Property {
        //@todo C++20, constexpr
        //@todo C++20 not possible yet, stdlibc++ does not have constexpr std::string
        static inline const PropertyIdType propertyId     = {lit.value};
        static inline const PropertyIdSequence idSequence = explode(propertyId, propertyDelimiter);

    private:
        std::optional<T> value;

    public:
        constexpr auto getPropertyName() const { return propertyId; }

        Property() requires detail::IsNotDirect<T> && (!IsContainerNotString<T>) {
            Parser fromStringParser;
            PropertyParserController controller;
            auto& parser = controller.parser();
            value        = fromStringParser.template fromString<T>(
                    parser.getValue(idSequence));//@todo this also pushes the stack, maybe we could avoid it.
        }

        Property() requires detail::IsNotDirect<T>&& IsContainerNotString<T> {
            PropertyParserController controller;
            auto& parser = controller.parser();
            parser.selectNode(idSequence);
            value = T{};
            while (parser.hasEntry()) {
                {
                    PropertyParser::StackKeeper keeper(parser);
                    value->insert(value->end(), typename T::value_type{});
                }
                parser.nextEntry();
            }
        }

        Property() requires detail::IsDirect<T> {
            PropertyParserController controller;
            value = detail::DirectPropertyMapper{.value = controller.parser().getValue(idSequence)};
        }

        const auto& getValue() const requires detail::IsNotDirect<T> { return *value; }

        template<typename TrueT>
        TrueT getValue() const requires detail::IsDirect<T> {
            Parser fromStringParser;
            return fromStringParser.template fromString<TrueT>(value->value);
        }

        const auto& operator()() { return getValue(); }

        template<typename TrueT>
        operator TrueT() {
            return getValue<TrueT>();
        }
    };

}// namespace bsc

#endif// BSC_PROPERTY_H
