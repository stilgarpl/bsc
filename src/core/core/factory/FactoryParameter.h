//
// Created by Krzysztof Tulidowicz on 08.12.2021.
//
#ifndef BSC_FACTORYPARAMETER_H
#define BSC_FACTORYPARAMETER_H

#include "parser/writer/toString.h"
#include <algorithm>
#include <core/factory/FactoryPtr.h>
#include <list>
#include <parser/parameters/CommandLineParameters.h>
#include <range/v3/to_container.hpp>
#include <ranges>

namespace bsc {

    namespace detail {

        template<typename FactoryType, bool hasArgumentType>
        struct FactoryBaseParameterDefinition {};

        template<typename FactoryType>
        struct FactoryBaseParameterDefinition<FactoryType, false> {};

        template<typename FactoryType>
        struct FactoryBaseParameterDefinition<FactoryType, true> {
            std::function<typename FactoryType::ArgumentType(void)> argumentFetcher = [] {return typename FactoryType::ArgumentType{};};
        };

    }// namespace detail

    class NoSelector : public std::invalid_argument {
    public:
        explicit NoSelector(const std::string& arg);
    };

    template<typename T, bool IsContainer>
    struct RemoveContainerType {};

    template<typename T>
    struct RemoveContainerType<T, true> {
        using Type = typename T::value_type;
    };

    template<typename T>
    struct RemoveContainerType<T, false> {
        using Type = T;
    };

    template<typename T>
    class FactoryParameter : public BaseParameter<T> {
        using ValueType = typename RemoveContainerType<T, IsContainerNotString<T>>::Type;
        FactoryPtr<ValueType> factory;
        using FactoryType = typename FactoryPtr<ValueType>::FactoryType;
        using SelectorType      = typename FactoryType::SelectorType;
        using SelectorValueType = std::conditional_t<IsContainerNotString<T>, std::list<SelectorType>, SelectorType>;

        std::optional<SelectorValueType> selectorValue;
        std::set<SelectorType> validSelectors;// do not optimize by removing this variable, check if ParsingFactory stopped generating
                                              // selectors set every time it calls getSelectors()

    public:
        struct ParameterDefinition {
            std::optional<char> shortKey{};
            std::optional<std::string_view> longKey{};
            std::optional<std::string_view> argumentName{};
            std::optional<std::string_view> doc{};
            std::optional<T> defaultValue{};
            decltype(selectorValue) defaultSelector{};
            FactoryPtr<ValueType> factory{};
            detail::FactoryBaseParameterDefinition<FactoryType, FactoryType::hasArgumentType> factoryDetails{};
        };

        typename BaseParameter<T>::OptionParseFunc makeParseFunction() {
            return [this](const char* input, Parser& parser) {
                std::string text = input != nullptr ? input : "";

                if constexpr (IsContainerNotString<T>) {
                    if (!this->selectorValue.has_value()) {
                        this->selectorValue.template emplace();
                    }

                    auto tempSelectorValue = parser.fromString<SelectorValueType>(text);

                    std::ranges::for_each(tempSelectorValue, [this, &text](auto& selector) {
                        validateSelector(text, selector);
                        this->selectorValue->insert(this->selectorValue->end(), selector);
                    });
                } else {
                    selectorValue = parser.fromString<SelectorValueType>(text);
                    validateSelector(text, *selectorValue);
                }

                this->incrementCounter();
            };
        }

        void validateSelector(const std::string& text, const SelectorType& selector) const {
            if (!validSelectors.contains(selector)) {
                using namespace std::string_literals;

                throw ValueNotAllowed("Value " + text + " is not allowed.", this->getAllowedValues());
            }
        }

        template<IsContainerNotString ContainerType>
        void validateSelector(const std::string& text, const ContainerType& selectorContainer) const {
            std::ranges::for_each(selectorContainer,[text, this](auto&& selector) {
                return validateSelector(text,selector);
            });
        }

        FactoryParameter(ParameterDefinition def)// NOLINT
            : BaseParameter<T>({.shortKey      = def.shortKey,
                                .longKey       = def.longKey,
                                .argumentName  = def.argumentName,
                                .doc           = def.doc,
                                .defaultValue  = def.defaultValue,
                                .allowedValues = selectorsToAllowedValues(def.factory->getSelectors())},
                               makeParseFunction()),
              factory(def.factory), selectorValue(def.defaultSelector), validSelectors(factory->getSelectors()), factoryDetails(def.factoryDetails) {
            if (def.defaultSelector) {
                validateSelector(toString(*def.defaultSelector), *def.defaultSelector );
            }
        }

        const auto& getSelector() const {
            return selectorValue;
        }


        typename BaseParameter<T>::ValueType operator()() const {
            typename BaseParameter<T>::ValueType result;
            if (selectorValue) {
                if constexpr (IsContainerNotString<T>) {
                    result.template emplace();

                    std::ranges::for_each(*this->selectorValue, [this, &result](auto& selector) {
                        result->insert(result->end(), this->createObject(selector));
                    });
                } else {
                    result = this->createObject(*selectorValue);
                }
                return result;
            } else {
                throw NoSelector("No selector provided for this factory");
            }
        }

    private:
        auto selectorsToAllowedValues(const auto& selectors) {
            return selectors | std::ranges::views::transform([](auto& i) {
                       static Writer writer;
                       return writer.template toString(i);
                   })
                   | ranges::to<typename BaseParameter<T>::AllowedValues::AllowedValuesSet>();
        }

        auto createObject(const SelectorType& selector) const {
            if constexpr (FactoryType::hasArgumentType) {
                return this->factory->create(selector, factoryDetails.argumentFetcher());
            } else {
                return this->factory->create(selector);
            }

        }
        decltype(ParameterDefinition::factoryDetails) factoryDetails;

    };
}// namespace bsc

#endif