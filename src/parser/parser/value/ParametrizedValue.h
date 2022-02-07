//
// Created by Krzysztof Tulidowicz on 31.01.2022.
//
#ifndef BSC_PARAMETRIZEDVALUE_H
#define BSC_PARAMETRIZEDVALUE_H

#include "parser/parser/fromString.h"
#include <concepts>
#include <fmt/format.h>
#include <regex>
#include <string_view>

namespace bsc {

    template<typename T>
    concept Affix = requires(T) {
        { T::present } -> std::convertible_to<bool>;
        { T::optional } -> std::convertible_to<bool>;
        { T::pattern } -> std::convertible_to<std::string>;
        typename T::ValueType;
    };

    namespace detail {
        class ParametrizedValueRegexFactory {

        public:
            //@todo C++20 those methods should be constexpr when std::string is constexpr (in GCC 12)
            template<typename T>
            constexpr static std::string_view makeRegexForType() {
                return "";//@todo implement
            }

            template<std::integral T>
            constexpr static std::string_view makeRegexForType() {
                return "[+-]?[[:digit:]]+";
            }

            template<std::floating_point T>
            constexpr static std::string_view makeRegexForType() {
                return "[+-]?[[:digit:]]+(?:\\.[[:digit:]]+)?";
            }
        };

        template<typename ValueType>
        class ParametrizedValuePrefixBase {
        private:
            std::optional<ValueType> value;

        protected:

            void setPrefix( std::optional<ValueType> v) {
                value = std::move(v);
            }

        public:

            const auto& getPrefix() {
                return value;
            }
        };

        template<>
        class ParametrizedValuePrefixBase<void> {};

        template<typename ValueType>
        class ParametrizedValuePostfixBase {
        private:
            std::optional<ValueType> value;

        protected:

            void setPostfix( std::optional<ValueType> v) {
                value = std::move(v);
            }

        public:
            const auto& getPostfix() {
                return value;
            }
        };

        template<>
        class ParametrizedValuePostfixBase<void> {};

    }// namespace detail

    template<typename ValueType, Affix auto prefix, Affix auto postfix>
    class ParametrizedValue : public detail::ParametrizedValuePrefixBase<typename decltype(prefix)::ValueType>,
                              public detail::ParametrizedValuePostfixBase<typename decltype(postfix)::ValueType> {

    private:
        constexpr static std::string_view valueRegexPattern =
                detail::ParametrizedValueRegexFactory::makeRegexForType<std::remove_cvref_t<ValueType>>();
        std::optional<std::remove_cvref_t<ValueType>> value;

        template<Affix AffixType>
        static constexpr bool affixNotVoid(AffixType affix) {
            if constexpr (std::is_same_v<void, typename AffixType::ValueType>) {
                return false;
            } else {
                return true;
            }
        }

        static std::string makePattern() {
            using namespace std::string_literals;
            std::string pattern = fmt::format("((?:{}){})((?:{}))((?:{}){})",
                                              prefix.present ? prefix.pattern : "",
                                              prefix.optional ? "?" : "",
                                              valueRegexPattern,
                                              postfix.present ? postfix.pattern : "",
                                              postfix.optional ? "?" : "");
            return pattern;
        }

        inline static const std::string regexPattern = makePattern();

    public:
        explicit ParametrizedValue(std::string textToParse) : ParametrizedValue(textToParse, Parser{}) {
        }

        ParametrizedValue(std::string textToParse, const Parser& fromStringParser) {
            std::smatch match;
            std::regex pattern{regexPattern};
            if (std::regex_match(textToParse, match, pattern)) {
                auto& prefixValue  = match[1];
                auto& valueText    = match[2];
                auto& postfixValue = match[3];

                value              = fromStringParser.template fromString<ValueType>(valueText.str());
                if constexpr (affixNotVoid(prefix)) {
                    if (!prefixValue.str().empty()) {
                        this->setPrefix(fromStringParser.template fromString<typename decltype(prefix)::ValueType>(prefixValue.str()));
                    }
                }
                
                if constexpr (affixNotVoid(postfix)) {
                    if (!postfixValue.str().empty()) {
                        this->setPostfix(fromStringParser.template fromString<typename decltype(postfix)::ValueType>(postfixValue.str()));
                    }
                }
            }
        }

        const auto& getValue() const {
            return value;
        }
    };
}// namespace bsc

#endif