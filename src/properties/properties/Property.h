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
#include <parser/writer/toString.h>
#include <properties/control/PropertyController.h>
#include <properties/control/writer/PropertyWriter.h>

namespace bsc {

    template<typename T>
    concept PropertyValue = !IsContainerNotString<T> && (IsStringNotClass<T> || ParsedFromString<T>);
    template<typename T>
    concept PropertyClass = !IsContainerNotString<T> && IsClassNotString<T>;
    template<typename T>
    concept PropertyContainer = IsContainerNotString<T>;

    template<detail::StringLiteral lit, typename T = detail::DirectPropertyMapper>
    class Property final {
        //@todo C++20, constexpr
        //@todo C++20 not possible yet, stdlibc++ does not have constexpr std::string
        static inline const PropertyIdType propertyId     = {lit.value};
        static inline const PropertyIdSequence idSequence = explode(propertyId, propertyDelimiter);
        static inline const Parser fromStringParser;
        static inline const Writer toStringWriter;

    private:
        std::optional<T> value;
        std::any directValue{};

    public:
        [[nodiscard]] bool hasValue() const {
            return value.has_value() || directValue.has_value();
        }

        constexpr auto getPropertyName() const {
            return propertyId;
        }

        explicit Property(const std::optional<T>& defaultValue) requires detail::IsNotDirect<T> && PropertyValue<T> {
            PropertyController controller;
            auto& parser = controller.parser();

            switch (parser.getNodeType(idSequence)) {

                case PropertyNodeType::sequence:
                case PropertyNodeType::map:
                    throw InvalidPropertyTypeException("Property key: " + propertyId + " has invalid node type");
                    break;
                default:
                    try {
                        value = fromStringParser.template fromString<T>(parser.getValue(idSequence));
                        break;
                    } catch (StringParseException& e) {
                        // fallthrough
                    }
                case PropertyNodeType::invalid:
                case PropertyNodeType::empty:
                    if (defaultValue.has_value()) {
                        value = defaultValue;
                    } else {
                        throw InvalidPropertyKeyException("Property key: " + propertyId +
                                                          " does not exist and default value was not provided.");
                    }
                    break;
            }
        }

        explicit Property(const std::optional<T>& defaultValue) requires detail::IsNotDirect<T> && PropertyClass<T> {
            PropertyController controller;
            auto& parser = controller.parser();
            parser.selectNode(idSequence);
            value = T{};// it should create itself
        }

        explicit Property(const std::optional<T>& defaultValue) requires detail::IsNotDirect<T> && PropertyContainer<T> {
            PropertyController controller;
            auto& parser = controller.parser();
            parser.selectNode(idSequence);
            if (parser.getNodeType() != PropertyNodeType::sequence) {
                if (defaultValue.has_value()) {
                    value = *defaultValue;
                    return;
                } else {
                    //@todo better error?
                    throw InvalidPropertyKeyException("Property key: " + propertyId +
                                                      " does not exist and default value was not provided.");
                }
            }
            value = T{};
            while (parser.hasEntry()) {
                {
                    PropertyStackKeeper<PropertyParser> keeper(parser);
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
            PropertyController controller;
            if (controller.parser().getNodeType(idSequence) == PropertyNodeType::scalar) {
                value = detail::DirectPropertyMapper{.value = controller.parser().getValue(idSequence)};
            }
            directValue = defaultValue;
        }

        Property() requires(detail::IsNotDirect<T>) : Property(std::nullopt) {
        }

        Property() requires(detail::IsDirect<T>) : Property(std::any{}) {
        }

//        explicit Property(const T& defaultValue) requires(!detail::IsDirect<T>) : Property(std::make_optional(defaultValue)){};

        const auto& getValue() const requires detail::IsNotDirect<T> {
            if (!hasValue()) {
                throw InvalidPropertyKeyException("Property key: " + propertyId + " does not exist and default value was not provided.");
            }
            return *value;
        }

        template<typename TrueT>
        TrueT getValue() const requires(detail::IsDirect<T>&& PropertyValue<TrueT>) {
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
        TrueT getValue() const requires(detail::IsDirect<T>&& PropertyClass<TrueT>) {
            PropertyController controller;
            controller.parser().selectNode(idSequence);
            return TrueT{};
        }

        template<typename TrueT>
        TrueT getValue() const requires(detail::IsDirect<T>&& PropertyContainer<TrueT>) {
            //@todo remove code duplication with non direct constructor
            PropertyController controller;
            auto& parser = controller.parser();
            parser.selectNode(idSequence);
            TrueT result = TrueT{};
            while (parser.hasEntry()) {
                {
                    PropertyStackKeeper<PropertyParser> keeper(parser);
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

        const auto& operator()() const {
            return getValue();
        }

        template<typename TrueT>
        operator TrueT() {
            return getValue<TrueT>();
        }

        auto& operator=(T t) {
            this->value = std::move(t);
            return *this;
        }

        void write(PropertyWriter& writer) const requires detail::IsNotDirect<T> && PropertyValue<T> {
            PropertyStackKeeper stackKeeper(writer);
            writer.selectNode(idSequence);
            writer.setValue(toStringWriter.template toString(getValue()));
        }

        void write(PropertyWriter& writer) const requires detail::IsNotDirect<T> && PropertyClass<T> && IsWritablePropertyClass<T> {
            PropertyStackKeeper stackKeeper(writer);
            writer.selectNode(idSequence);
            PropertySequencer sequencer(writer);
            getValue().write(sequencer);
        }

        void write(PropertyWriter& writer) const requires detail::IsNotDirect<T> && PropertyContainer<T>;
    };

    template<typename T>
    struct IsPropertyT : std::false_type {};

    template<detail::StringLiteral lit, typename T>
    struct IsPropertyT<Property<lit, T>> : std::true_type {};

    //    template<typename T>
    //    constexpr bool is_pair_v = is_pair<T>::value;
    template<typename T>
    concept IsProperty = IsPropertyT<T>::value;

    template<detail::StringLiteral lit, typename T>
    void Property<lit, T>::write(PropertyWriter& writer) const requires detail::IsNotDirect<T> && PropertyContainer<T> {
        PropertyStackKeeper stackKeeper(writer);
        writer.selectNode(idSequence);
        writer.setNodeType(PropertyNodeType::sequence);
        for (auto& item : getValue()) {
            if constexpr (IsProperty<typename T::value_type> || PropertyClass<typename T::value_type>) {
                PropertyStackKeeper stackKeeper(writer);
                item.write(writer);
                writer.nextEntry();
            } else {
                // collection of simple types
                writer.setValue(toStringWriter.template toString(item));
                writer.nextEntry();
            }
        }
    }

}// namespace bsc

#endif// BSC_PROPERTY_H
