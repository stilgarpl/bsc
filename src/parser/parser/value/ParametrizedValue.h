//
// Created by Krzysztof Tulidowicz on 31.01.2022.
//
#ifndef BSC_PARAMETRIZEDVALUE_H
#define BSC_PARAMETRIZEDVALUE_H

#include "parser/parser/fromString.h"
#include <fmt/format.h>
#include <regex>
#include "Affix.h"
#include <string_view>

namespace bsc {

    namespace detail {
        class ParametrizedValueRegexFactory {

        public:
            //@todo C++20 those methods should be constexpr when std::string is constexpr (in GCC 12)
            template<typename T>
            constexpr static std::string_view makeRegexForType() {
                return "(?:.*)";
            }

            template<std::integral T>
            constexpr static std::string_view makeRegexForType() {
                return "[[:digit:]]+";
            }

            template<std::floating_point T>
            constexpr static std::string_view makeRegexForType() {
                return "[[:digit:]]+(?:\\.[[:digit:]]+)?";
            }
        };

    }// namespace detail

    template<typename ValueType, affixes::IsAffix PrefixType, affixes::IsAffix PostfixType>
    class ParametrizedValue {

    private:
        static constexpr bool hasPrefixValue = (!std::is_void_v<typename PrefixType::ValueType>);
        static constexpr bool hasPostfixValue = (!std::is_void_v<typename PostfixType::ValueType>);

        static constexpr bool isPrefixTransform = affixes::IsValueTransformAffix<PrefixType, ValueType>;
        static constexpr bool isPostfixTransform = affixes::IsValueTransformAffix<PostfixType, ValueType>;

        static constexpr bool isPrefixCompare = affixes::IsRelationalAffix<PrefixType, ValueType>;
        static constexpr bool isPostfixCompare = affixes::IsRelationalAffix<PostfixType, ValueType>;

        PrefixType prefix{};
        PostfixType postfix{};
        constexpr static std::string_view valueRegexPattern =
                detail::ParametrizedValueRegexFactory::makeRegexForType<std::remove_cvref_t<ValueType>>();
        std::optional<std::remove_cvref_t<ValueType>> value;

        void setPrefix(std::optional<typename PrefixType::ValueType> v) requires hasPrefixValue {
            prefix.value = v;
        }

        void setPostfix(std::optional<typename PostfixType::ValueType> v) requires hasPostfixValue {
            postfix.value = v;
        }

        std::string makePattern() {
            using namespace std::string_literals;
            //@todo make it static for type, we only need one pattern per ParametrizedValue type
            std::string pattern = fmt::format("((?:{}){})((?:{}))((?:{}){})",
                                              prefix.present ? prefix.pattern : "",
                                              prefix.optional ? "?" : "",
                                              valueRegexPattern,
                                              postfix.present ? postfix.pattern : "",
                                              postfix.optional ? "?" : "");
            return pattern;
        }

        //@todo make it static for type, we only need one pattern per ParametrizedValue type
        const std::string regexPattern = makePattern();

    public:


        const auto & getPrefix() requires (!std::is_void_v<typename PrefixType::ValueType>) {
            return prefix.value;
        }

        const auto & getPostfix() requires (!std::is_void_v<typename PostfixType::ValueType>) {
            return postfix.value;
        }

        ParametrizedValue(const std::string& textToParse) : ParametrizedValue(textToParse, Parser{}) {
        }

        ParametrizedValue(const std::string& textToParse, const Parser& fromStringParser) {
            std::smatch match;
            std::regex pattern{regexPattern};
            if (std::regex_match(textToParse, match, pattern)) {
                auto& prefixValue  = match[1];
                auto& valueText    = match[2];
                auto& postfixValue = match[3];

                value              = fromStringParser.template fromString<ValueType>(valueText.str());
                if constexpr (hasPrefixValue) {
                    if (!prefixValue.str().empty()) {
                        this->setPrefix(fromStringParser.template fromString<typename PrefixType::ValueType>(prefixValue.str()));
                    }
                }
                
                if constexpr (hasPostfixValue) {
                    if (!postfixValue.str().empty()) {
                        this->setPostfix(fromStringParser.template fromString<typename PostfixType::ValueType>(postfixValue.str()));
                    }
                }
            }
        }

        const auto& getRawValue() const {
            return value;
        }

        auto getValue() const -> ValueType  {
            //@todo this should be computed during parsing phase and stored as a field, not computed on every call. future optimization, get it working first
            if constexpr (isPrefixTransform && isPostfixTransform) {
                return postfix.transform(prefix.transform(*value));
            } else
            if constexpr (isPrefixTransform ) {
                return prefix.transform(*value);
            } else
            if constexpr (isPostfixTransform) {
                return postfix.transform(*value);
            } else {
                return *value;//@todo handle errors?
            }
        }

        bool compare(ValueType v) const requires (isPrefixCompare or isPostfixCompare){
            bool result = false;
            if (value.has_value() ) {
                if constexpr (isPrefixCompare) {
                    result = result or prefix.compare(v, getValue());
                }
                if constexpr (isPostfixCompare) {
                    result = result or postfix.compare(getValue(), v);
                }
            }
            return result;
        }
    };
}// namespace bsc

#endif