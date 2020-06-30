//
// Created by stilgar on 08.01.2020.
//

#ifndef BSC_FROMSTRING_H
#define BSC_FROMSTRING_H

#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace bsc {

    //@todo C++20 all of those SFINAE enable_if can be replaced by concepts, but concepts do not quite work yet in
    //current
    // compilers. try again when GCC 11 comes out.
    template<typename T>
    struct IsPairT : std::false_type {};

    template<typename T, typename U>
    struct IsPairT<std::pair<T, U>> : std::true_type {};

    //    template<typename T>
    //    constexpr bool is_pair_v = is_pair<T>::value;
    template<typename T>
    concept IsPair = IsPairT<T>::value;

    template<typename T, typename _ = void>
    struct IsContainerNotStringCheck : std::false_type {};

    template<typename T>
    struct IsContainerNotStringCheck<T,
                                   std::void_t<typename T::value_type,
                                               typename T::size_type,
                                               typename T::allocator_type,
                                               typename T::iterator,
                                               typename T::const_iterator,
                                               decltype(std::declval<T>().size()),
                                               decltype(std::declval<T>().begin()),
                                               decltype(std::declval<T>().end()),
                                               decltype(std::declval<T>().cbegin()),
                                               decltype(std::declval<T>().cend()),
                                               std::enable_if_t<!std::is_convertible_v<T, std::string>>

                                               >> : public std::true_type {};
    template<typename T>
    concept IsContainerNotString = IsContainerNotStringCheck<T>::value;
    /*std::negation_v<std::is_convertible<T, std::string>> && requires {
            typename T::value_type;
                    typename T::size_type;
                    typename T::allocator_type;
                    typename T::iterator;
                    typename T::const_iterator;
                    std::declval<T>().size();
                    std::declval<T>().begin();
                    std::declval<T>().end();
                    std::declval<T>().cbegin();
                    std::declval<T>().cend();
        };*/

    class StringParseException : public std::invalid_argument {
    public:
        explicit StringParseException(const std::string& arg) : invalid_argument(arg) {}
    };

    class Parser {
    public:
        struct ParserConfiguration {
            char csvDelimiter  = ',';
            char pairDelimiter = '=';
        };

    private:
        ParserConfiguration parserConfiguration;

    public:
        explicit Parser(const ParserConfiguration& parserConfiguration) : parserConfiguration(parserConfiguration) {}
        Parser() = default;

    public:
        template<typename ParameterType>
        [[nodiscard]] std::remove_reference_t<ParameterType>
        fromString(const std::string& value,
                   std::enable_if_t<std::numeric_limits<ParameterType>::is_integer, int> = 0) const  {
            try {
                return std::stol(value);
            } catch (std::invalid_argument& e) {
                throw StringParseException("Long parsing failed for value: " + value);
            }
        }

        template<typename ParameterType>
        [[nodiscard]] std::remove_reference_t<ParameterType>
        fromString(const std::string& value, std::enable_if_t<std::is_floating_point_v<ParameterType>, int> = 0) const {
            try {
                return std::stod(value);
            } catch (std::invalid_argument& e) {
                throw StringParseException("Floating parsing failed for value: " + value);
            }
        }

        template<typename ParameterType>
        [[nodiscard]] std::remove_reference_t<ParameterType>
        fromString(const std::string& value,
                   std::enable_if_t<std::is_convertible_v<ParameterType, std::string>, int> = 0) const {
            if (!value.empty()) {
                return ParameterType(value);
            } else {
                throw StringParseException("Can't parse null parameter value");
            }
        }

        template<typename ParameterType>
        [[nodiscard]] std::remove_reference_t<ParameterType> fromString(const std::string& value,
                                                          int = 0) const requires IsPair<ParameterType> {
            try {

                std::stringstream inputStream(value);
                std::string first, second;
                getline(inputStream, first, parserConfiguration.pairDelimiter);
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
        fromString(const std::string& value, std::enable_if_t<IsContainerNotStringCheck<ParameterType>::value, int> = 0) const {
            ParameterType container;
            std::stringstream inputStream(value);
            std::string element;

            while (getline(inputStream, element, parserConfiguration.csvDelimiter)) {
                container.insert(container.end(), fromString<typename ParameterType::value_type>(element.c_str()));
            }

            return container;
        }
    };

    template<>
    [[nodiscard]] inline bool Parser::fromString<bool>(const std::string& value, int)const {
        return true;
    }

    template<>
    [[nodiscard]] inline int Parser::fromString<int>(const std::string& value, int) const{
        try {
            return std::stoi(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Integer parsing failed for value: " + value);
        }
    }

    template<>
    [[nodiscard]] inline long Parser::fromString<long>(const std::string& value, int)const {
        try {
            return std::stol(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Long parsing failed for value: " + value);
        }
    }

    template<>
    [[nodiscard]] inline unsigned long Parser::fromString<unsigned long>(const std::string& value, int)const {
        try {
            return std::stoul(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Unsigned long parsing failed for value: " + value);
        }
    }

    template<>
    [[nodiscard]] inline float Parser::fromString<float>(const std::string& value, int) const{
        try {
            return std::stof(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Floating parsing failed for value: " + value);
        }
    }

    template<>
    [[nodiscard]] inline double Parser::fromString<double>(const std::string& value, int) const{
        try {
            return std::stod(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Floating parsing failed for value: " + value);
        }
    }

    template<>
    [[nodiscard]] inline long double Parser::fromString<long double>(const std::string& value, int) const {
        try {
            return std::stold(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Floating parsing failed for value: " + value);
        }
    }

    template<typename ParameterType>
    [[nodiscard]] auto fromString(const std::string& value) {
        static Parser parser;
        return parser.fromString<ParameterType>(value);
    }

}// namespace bsc

#endif// BSC_FROMSTRING_H
