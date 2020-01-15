//
// Created by stilgar on 08.01.2020.
//

#ifndef BASYCO_FROMSTRING_H
#define BASYCO_FROMSTRING_H

#include <type_traits>
#include <limits>
#include <string>
#include <stdexcept>
#include <sstream>


//template<typename T>
//class that_type;
//
//template<typename T>
//void name_that_type() {
//    that_type<T> tType;
//}

namespace bsc {

//@todo instead of SFINAE maybe I should use just one function with constexpr if() that calls itself when needed? that would solve a problem of ambigious calls when more than one function fits
//  basically, template<ParamType> ParamType fromString(value) {if ... return fromString<..>() else if ... else if.. }; and template specializations for specific types.
    template<typename T>
    struct is_pair : std::false_type {
    };

    template<typename T, typename U>
    struct is_pair<std::pair<T, U>> : std::true_type {
    };

    template<typename T>
    constexpr bool is_pair_v = is_pair<T>::value;

    template<typename T, typename _ = void>
    struct is_container_not_string : std::false_type {
    };

    template<typename T>
    struct is_container_not_string<
            T,
            std::void_t<
                    typename T::value_type,
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

            >
    > : public std::true_type {
    };

    class StringParseException : public std::invalid_argument {
    public:
        explicit StringParseException(const std::string& arg) : invalid_argument(arg) {}
    };

    template<typename ParameterType>
    std::remove_reference_t<ParameterType>
    fromString(const std::string& value, std::enable_if_t<std::numeric_limits<ParameterType>::is_integer, int> = 0) {
        //@todo stoi, stol, stoii based on Parameter type length?
        try {
            return std::stol(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Integer parsing failed for value: " + value);
        }
    }

    template<typename ParameterType>
    std::remove_reference_t<ParameterType>
    fromString(const std::string& value, std::enable_if_t<std::is_floating_point_v<ParameterType>, int> = 0) {
        //@todo stoi, stol, stoii based on Parameter type length?
        try {
            return std::stod(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Floating parsing failed for value: " + value);
        }
    }


    template<typename ParameterType>
    std::remove_reference_t<ParameterType>
    fromString(const std::string& value,
               std::enable_if_t<std::is_convertible_v<ParameterType, std::string>, int>  = 0) {
        if (!value.empty()) {
            return ParameterType(value);
        } else {
            throw StringParseException("Can't parse null parameter value");
        }

    }

    template<>
    inline bool fromString<bool>(const std::string& value, int) {
        return true;
    }

    template<>
    inline int fromString<int>(const std::string& value, int) {
        try {
            return std::stoi(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Integer parsing failed for value: " + value);
        }
    }

    template<>
    inline long fromString<long>(const std::string& value, int) {
        try {
            return std::stol(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Integer parsing failed for value: " + value);
        }
    }

    template<>
    inline unsigned long fromString<unsigned long>(const std::string& value, int) {
        try {
            return std::stoul(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Integer parsing failed for value: " + value);
        }
    }


    template<>
    inline float fromString<float>(const std::string& value, int) {
        try {
            return std::stof(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Floating parsing failed for value: " + value);
        }
    }

    template<>
    inline double fromString<double>(const std::string& value, int) {
        try {
            return std::stod(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Floating parsing failed for value: " + value);
        }
    }

    template<>
    inline long double fromString<long double>(const std::string& value, int) {
        try {
            return std::stold(value);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Floating parsing failed for value: " + value);
        }
    }


    template<typename ParameterType>
    std::remove_reference_t<ParameterType>
    fromString(const std::string& value, std::enable_if_t<is_pair_v<ParameterType>, int> = 0) {
        try {

            std::stringstream inputStream(value);
            std::string first, second;
            getline(inputStream, first, '=');
            getline(inputStream, second);
            auto key = fromString<std::decay_t<typename ParameterType::first_type>>(first.c_str());
            auto pairValue = fromString<typename ParameterType::second_type>(second.c_str());
            return std::make_pair(key, pairValue);
        } catch (std::invalid_argument& e) {
            throw StringParseException("Pair parsing failed for value: " + value);
        }
    }

    template<typename ParameterType>
    std::remove_reference_t<ParameterType>
    fromString(const std::string& value, std::enable_if_t<is_container_not_string<ParameterType>::value, int>  = 0) {
        ParameterType container;
        std::stringstream inputStream(value);
        std::string element;

        while (getline(inputStream, element, ',')) { ;
            container.insert(container.end(), fromString<typename ParameterType::value_type>(element.c_str()));
        }

        return container;

    }

}

#endif //BASYCO_FROMSTRING_H
