//
// Created by Krzysztof Tulidowicz on 08.01.2020.
//

#ifndef BSC_FROMSTRING_H
#define BSC_FROMSTRING_H

#include <limits>
#include <memory>
#include <parser/concepts/concepts.h>
#include <parser/configuration/ParserConfiguration.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
//@todo make it optional on cmake option to use magic_enum
#include <magic_enum.hpp>

namespace bsc {

    class StringParseException : public std::invalid_argument {
    public:
        explicit StringParseException(const StringType& arg) : invalid_argument(arg) {
        }
    };

    class Parser;

    namespace detail {
        struct DefaultParserImplementation {

            Parser* parser;
            //@todo parserConfiguration can be removed since we have parser reference
            std::shared_ptr<ParserConfiguration> parserConfiguration = std::make_shared<ParserConfiguration>();


        public:
            explicit DefaultParserImplementation(Parser* p, std::shared_ptr<ParserConfiguration> parserConfiguration)
                : parser(p), parserConfiguration(std::move(parserConfiguration)) {
            }

        public: //I'd prefer this to be private, but concept HasDefaultImplementation does not work with private (it doesn't see friend)
            friend class Parser;

            template<typename ParameterType>
            [[nodiscard]] std::remove_reference_t<ParameterType>
            fromStringImpl(const StringType& value) const requires std::is_floating_point_v<ParameterType> {
                try {
                    //@todo use "if constexpr" to determine the size of ParameterType and use appropriate std function
                    return std::stod(value);
                } catch (const std::invalid_argument& e) {
                    throw StringParseException("Floating parsing failed for value: " + value);
                }
            }

            template<IsBool ParameterType>
            [[nodiscard]] bool fromStringImpl(const StringType& value) const {
                //@todo maybe check if string is not empty and then if it has values like "false" or something
                return true;
            }

            template<typename ParameterType>
            [[nodiscard]] std::remove_reference_t<ParameterType>
            fromStringImpl(const StringType& value) const requires std::is_integral_v<ParameterType> &&(!IsBool<ParameterType>) {
                try {
                    //@todo use "if constexpr" to determine the size of ParameterType and use appropriate std function
                    return std::stoll(value);
                } catch (const std::invalid_argument& e) {
                    throw StringParseException("Integer parsing failed for value: " + value);
                }
            }

            template<typename ParameterType>
            [[nodiscard]] std::remove_reference_t<ParameterType>
            fromStringImpl(const StringType& value) const requires std::is_enum_v<ParameterType> {
                auto optionalEnum = magic_enum::enum_cast<ParameterType>(value);
                if (optionalEnum) {
                    return *optionalEnum;
                } else {
                    throw StringParseException("Enum parsing failed for value: " + value);
                }
            }

            template<typename ParameterType>
            [[nodiscard]] std::remove_reference_t<ParameterType>
            fromStringImpl(const StringType& value) const requires std::is_convertible_v<ParameterType, StringType> {
                if (!value.empty()) {
                    return ParameterType(value);
                } else {
                    throw StringParseException("Can't parse null parameter value");
                }
            }

            template<IsPair ParameterType>
            [[nodiscard]] std::remove_reference_t<ParameterType> fromStringImpl(const StringType& value) const {
                try {
                    std::stringstream inputStream(value);
                    StringType first, second;
                    getline(inputStream, first, parserConfiguration->pairDelimiter);
                    getline(inputStream, second);
                    auto key       = fromString<std::decay_t<typename ParameterType::first_type>>(first.c_str());
                    auto pairValue = fromString<typename ParameterType::second_type>(second.c_str());
                    return std::make_pair(key, pairValue);
                } catch (const std::invalid_argument& e) {
                    throw StringParseException("Pair parsing failed for value: " + value);
                }
            }

            template<IsContainerNotString ParameterType>
            [[nodiscard]] std::remove_reference_t<ParameterType> fromStringImpl(const StringType& value) const;
        };


        template<typename T>
        concept HasDefaultParserImplementation = requires(const DefaultParserImplementation& parser, const StringType& s) {
            parser.fromStringImpl<T>(s);
        };
    }// namespace detail

    template<typename ParameterType>
    class ParserImplementation {
        //        [[nodiscard]] std::remove_reference_t<ParameterType>
        //        parse(const StringType& value) {
        //
        //        }
    };

    template<typename ParameterType>
    concept HasCustomParserImplementation = requires(ParserImplementation<ParameterType> p, StringType s) {
        {p.parse(s)} -> std::same_as<std::remove_reference_t<ParameterType>>;
    };

    class Parser {
    private:
        std::shared_ptr<ParserConfiguration> parserConfiguration = std::make_shared<ParserConfiguration>();
        detail::DefaultParserImplementation defaultParserImplementation{this, parserConfiguration};

    public:
        explicit Parser(const ParserConfiguration& parserConfiguration)
            : parserConfiguration(std::make_shared<ParserConfiguration>(parserConfiguration)), defaultParserImplementation{this,
                                                                                                       this->parserConfiguration} {
        }
        Parser() = default;
        template<typename ParameterType>
        [[nodiscard]] std::remove_cvref_t<ParameterType> fromString(StringType value)
                const requires detail::HasDefaultParserImplementation<std::remove_cvref_t<ParameterType>> || HasCustomParserImplementation<std::remove_cvref_t<ParameterType>> {
            if constexpr (HasCustomParserImplementation<std::remove_cvref_t<ParameterType>>) {
                ParserImplementation<ParameterType> impl;
                return impl.parse(value);
            } else {
                return defaultParserImplementation.fromStringImpl<std::remove_cvref_t<ParameterType>>(std::move(value));
            }
        }
    };

    template<typename ParameterType>
    [[nodiscard]] auto fromString(const StringType& value) {
        static Parser parser;
        return parser.fromString<ParameterType>(value);
    }

    template<typename T>
    concept ParsedFromString = requires(const Parser& parser, const StringType& s) {
        parser.fromString<T>(s);
    }
    || HasCustomParserImplementation<T>;

    namespace detail {
        template<IsContainerNotString ParameterType>
        std::remove_reference_t<ParameterType> DefaultParserImplementation::fromStringImpl(const StringType& value) const {
            ParameterType container;
            std::stringstream inputStream(value);
            StringType element;

            while (getline(inputStream, element, parserConfiguration->csvDelimiter)) {
                container.insert(container.end(), parser->fromString<typename ParameterType::value_type>(element.c_str()));
            }

            return container;
        }
    }

}// namespace bsc

#endif// BSC_FROMSTRING_H
