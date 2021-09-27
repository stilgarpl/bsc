//
// Created by Krzysztof Tulidowicz on 08.01.2020.
//

#ifndef BSC_FROMSTRING_H
#define BSC_FROMSTRING_H

#include <limits>
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

    class Parser {
    private:
        std::shared_ptr<ParserConfiguration> parserConfiguration = std::make_shared<ParserConfiguration>();

    public:
        explicit Parser(const ParserConfiguration& parserConfiguration)
            : parserConfiguration(std::make_shared<ParserConfiguration>(parserConfiguration)) {
        }
        Parser() = default;

    public:
        template<typename ParameterType>
        [[nodiscard]] std::remove_reference_t<ParameterType>
        fromString(const StringType& value, std::enable_if_t<std::numeric_limits<ParameterType>::is_integer, int> = 0) const {
            try {
                return std::stoll(value);
            } catch (std::invalid_argument& e) {
                throw StringParseException("Long parsing failed for value: " + value);
            }
        }

        template<typename ParameterType>
        [[nodiscard]] std::remove_reference_t<ParameterType>
        fromString(const StringType& value, std::enable_if_t<std::is_floating_point_v<ParameterType>, int> = 0) const {
            try {
                return std::stod(value);
            } catch (std::invalid_argument& e) {
                throw StringParseException("Floating parsing failed for value: " + value);
            }
        }

        template<typename ParameterType>
        [[nodiscard]] std::remove_reference_t<ParameterType>
        fromString(const StringType& value, std::enable_if_t<std::is_enum_v<ParameterType>, int> = 0) const {
            auto optionalEnum = magic_enum::enum_cast<ParameterType>(value);
            if (optionalEnum) {
                return *optionalEnum;
            } else {
                throw StringParseException("Enum parsing failed for value: " + value);
            }
        }

        template<typename ParameterType>
        [[nodiscard]] std::remove_reference_t<ParameterType>
        fromString(const StringType& value, std::enable_if_t<std::is_convertible_v<ParameterType, StringType>, int> = 0) const {
            if (!value.empty()) {
                return ParameterType(value);
            } else {
                throw StringParseException("Can't parse null parameter value");
            }
        }

        template<typename ParameterType>
        [[nodiscard]] std::remove_reference_t<ParameterType> fromString(const StringType& value,
                                                                        int = 0) const requires IsPair<ParameterType> {
            try {

                std::stringstream inputStream(value);
                StringType first, second;
                getline(inputStream, first, parserConfiguration->pairDelimiter);
                getline(inputStream, second);
                auto key       = fromString<std::decay_t<typename ParameterType::first_type>>(first.c_str());
                auto pairValue = fromString<typename ParameterType::second_type>(second.c_str());
                return std::make_pair(key, pairValue);
            } catch (std::invalid_argument& e) {
                throw StringParseException("Pair parsing failed for value: " + value);
            }
        }

        template<typename ParameterType>
        [[nodiscard]] std::remove_reference_t<ParameterType>
        fromString(const StringType& value, std::enable_if_t<IsContainerNotStringCheck<ParameterType>::value, int> = 0) const {
            ParameterType container;
            std::stringstream inputStream(value);
            StringType element;

            while (getline(inputStream, element, parserConfiguration->csvDelimiter)) {
                container.insert(container.end(), fromString<typename ParameterType::value_type>(element.c_str()));
            }

            return container;
        }
    };

    template<>
    [[nodiscard]] inline bool Parser::fromString<bool>(const StringType& value, int) const {
        return true;
    }

    template<>
    [[nodiscard]] inline int Parser::fromString<int>(const StringType& value, int) const {
        try {
            return std::stoi(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Integer parsing failed for value: " + value);
        }
    }

    template<>
    [[nodiscard]] inline long Parser::fromString<long>(const StringType& value, int) const {
        try {
            return std::stol(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Long parsing failed for value: " + value);
        }
    }

    template<>
    [[nodiscard]] inline unsigned long Parser::fromString<unsigned long>(const StringType& value, int) const {
        try {
            return std::stoul(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Unsigned long parsing failed for value: " + value);
        }
    }

    template<>
    [[nodiscard]] inline float Parser::fromString<float>(const StringType& value, int) const {
        try {
            return std::stof(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Floating parsing failed for value: " + value);
        }
    }

    template<>
    [[nodiscard]] inline double Parser::fromString<double>(const StringType& value, int) const {
        try {
            return std::stod(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Floating parsing failed for value: " + value);
        }
    }

    template<>
    [[nodiscard]] inline long double Parser::fromString<long double>(const StringType& value, int) const {
        try {
            return std::stold(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Floating parsing failed for value: " + value);
        }
    }

    template<typename ParameterType>
    [[nodiscard]] auto fromString(const StringType& value) {
        static Parser parser;
        return parser.fromString<ParameterType>(value);
    }

    template<typename T>
    concept ParsedFromString = requires(const Parser& parser, const std::string& s) {
        parser.fromString<T>(s);
    };

}// namespace bsc

#endif// BSC_FROMSTRING_H
