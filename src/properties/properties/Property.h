//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//

#ifndef BSC_PROPERTY_H
#define BSC_PROPERTY_H

#include "Property.h"
#include "PropertyContext.h"
#include "PropertyDefinitions.h"
#include "PropertyExceptions.h"
#include "PropertyPrefix.h"
#include <algorithm>
#include <any>
#include <optional>
#include <parser/parser/explode.h>
#include <parser/parser/fromString.h>
#include <properties/parser/PropertyParserController.h>

namespace bsc {

    template<detail::StringLiteral lit, typename T = detail::DirectPropertyMapper>
    class Property final {
        //@todo C++20, constexpr
        //@todo C++20 not possible yet, stdlibc++ does not have constexpr std::string
        static inline const PropertyIdType propertyId     = {lit.value};
        static inline const PropertyIdSequence idSequence = explode(propertyId, propertyDelimiter);
        static inline const Parser fromStringParser;

    private:
        std::optional<T> value;
        std::any directValue{};

    public:
        [[nodiscard]] bool hasValue() const { return value.has_value() || directValue.has_value(); }

        constexpr auto getPropertyName() const { return propertyId; }

        explicit Property(const std::optional<T>& defaultValue) requires detail::IsNotDirect<T> &&
                (!IsContainerNotString<T> && (!std::is_class_v<T> || IsString<T>) ) {
            PropertyParserController controller;
            auto& parser = controller.parser();

            switch (parser.getNodeType(idSequence)) {

                case PropertyParserNodeType::sequence:
                case PropertyParserNodeType::map:
                    throw InvalidPropertyTypeException("Property key: " + propertyId + " has invalid node type");
                    break;
                default:
                    try {
                        value = fromStringParser.template fromString<T>(parser.getValue(idSequence));
                        break;
                    } catch (StringParseException& e) {
                        // fallthrough
                    }
                case PropertyParserNodeType::invalid:
                case PropertyParserNodeType::empty:
                    if (defaultValue.has_value()) {
                        value = defaultValue;
                    } else {
                        throw InvalidPropertyKeyException("Property key: " + propertyId +
                                                          " does not exist and default value was not provided.");
                    }
                    break;
            }
        }

        explicit Property(const std::optional<T>& defaultValue) requires detail::IsNotDirect<T> &&
                (!IsContainerNotString<T> && std::is_class_v<T> && !IsString<T>) {
            PropertyParserController controller;
            auto& parser = controller.parser();
            parser.selectNode(idSequence);
            value = T{};// it should create itself
        }

        explicit Property(const std::optional<T>& defaultValue) requires detail::IsNotDirect<T>&& IsContainerNotString<T> {
            PropertyParserController controller;
            auto& parser = controller.parser();
            parser.selectNode(idSequence);
            value = T{};
            while (parser.hasEntry()) {
                {
                    PropertyParser::StackKeeper keeper(parser);
                    if constexpr (IsClassNotString<typename T::value_type>) {
                        value->insert(value->end(), typename T::value_type{});
                    } else {
                        value->insert(value->end(), fromStringParser.template fromString<typename T::value_type>(parser.getValue()));
                    }
                }
                parser.nextEntry();
            }
        }

        explicit Property(const std::any& defaultValue) requires detail::IsDirect<T> {
            PropertyParserController controller;
            if (controller.parser().getNodeType(idSequence) == PropertyParserNodeType::scalar) {
                value = detail::DirectPropertyMapper{.value = controller.parser().getValue(idSequence)};
            }
            directValue = defaultValue;
        }

        Property() requires(!detail::IsDirect<T>) : Property(std::nullopt) {}

        Property() requires(detail::IsDirect<T>) : Property(std::any{}) {}

        explicit Property(const T& defaultValue) requires(!detail::IsDirect<T>) : Property(std::make_optional(defaultValue)){};

        const auto& getValue() const requires detail::IsNotDirect<T> {
            if (!hasValue()) {
                throw InvalidPropertyKeyException("Property key: " + propertyId + " does not exist and default value was not provided.");
            }
            return *value;
        }

        template<typename TrueT>
        TrueT getValue() const requires(detail::IsDirect<T> && !(IsClassNotString<TrueT>) ) {
            if (!hasValue()) {
                throw InvalidPropertyKeyException("Property key: " + propertyId + " does not exist and default value was not provided.");
            }
            if (!value.has_value()) {
                return std::any_cast<TrueT>(directValue);
            }
            try {
                Parser fromStringParser;
                return fromStringParser.template fromString<TrueT>(value->value);
            } catch (StringParseException& e) {
                return std::any_cast<TrueT>(directValue);
            }
        }

        template<typename TrueT>
        TrueT getValue() const requires(detail::IsDirect<T> && (IsClassNotString<TrueT> && !IsContainerNotString<TrueT>) ) {
            PropertyParserController controller;
            controller.parser().selectNode(idSequence);
            return TrueT{};
        }

        template<typename TrueT>
        TrueT getValue() const requires(detail::IsDirect<T>&& IsContainerNotString<TrueT>) {
            //@todo remove code duplication with non direct constructor
            PropertyParserController controller;
            auto& parser = controller.parser();
            parser.selectNode(idSequence);
            TrueT result = TrueT{};
            while (parser.hasEntry()) {
                {
                    PropertyParser::StackKeeper keeper(parser);
                    if constexpr (IsClassNotString<typename TrueT::value_type>) {
                        result.insert(result.end(), typename TrueT::value_type{});
                    } else {
                        result.insert(result.end(), fromStringParser.template fromString<typename TrueT::value_type>(parser.getValue()));
                    }
                }
                parser.nextEntry();
            }
            return result;
        }

        const auto& operator()() const { return getValue(); }

        template<typename TrueT>
        operator TrueT() {
            return getValue<TrueT>();
        }
    };

}// namespace bsc

#endif// BSC_PROPERTY_H
