//
// Created by stilgar on 08.01.2020.
//

#ifndef BASYCO_PARAMETERPARSE_H
#define BASYCO_PARAMETERPARSE_H

#include <type_traits>
#include <limits>
#include <string>
#include <stdexcept>
#include <sstream>

template<typename T, typename _ = void>
struct is_container : std::false_type {
};

template<typename... Ts>
struct is_container_helper {
};

template<typename T>
struct is_container<
        T,
        std::enable_if_t<
                std::conjunction_v<is_container_helper<
                        typename T::value_type,
                        typename T::size_type,
                        typename T::allocator_type,
                        typename T::iterator,
                        typename T::const_iterator,
                        decltype(std::declval<T>().size()),
                        decltype(std::declval<T>().begin()),
                        decltype(std::declval<T>().end()),
                        decltype(std::declval<T>().cbegin()),
                        decltype(std::declval<T>().cend())
                >::value, std::negation_v<std::is_same_v<T, std::string>>
                >
        >> : public std::true_type {
};

class ParameterParseException : public std::invalid_argument {
public:
    explicit ParameterParseException(const std::string& arg) : invalid_argument(arg) {}
};

template<typename ParameterType>
ParameterType parse(const char* value, std::enable_if_t<std::numeric_limits<ParameterType>::is_integer, int> = 0) {
    //@todo stoi, stol, stoii based on Parameter type length?
    try {
        return std::stol(value);
    } catch (std::invalid_argument& e) {
        throw ParameterParseException("Integer parsing failed.");
    }
}

template<typename ParameterType>
ParameterType parse(const char* value, std::enable_if_t<std::is_convertible_v<ParameterType, std::string>, int>  = 0) {
    if (value != nullptr) {
        return ParameterType(value);
    } else {
        throw ParameterParseException("Can't parse null parameter value");
    }

}

template<>
inline bool parse<bool>(const char* value, int) {
    return true;
}

template<typename ParameterType>
inline ParameterType parse(const char* value, std::enable_if_t<is_container<ParameterType>::value, int>  = 0) {
    ParameterType container;
    std::stringstream s;
    std::string val;

    while (getline(s, val, ','))
        container.push_back(parse<typename ParameterType::value_type>(val));

    return container;

}

#endif //BASYCO_PARAMETERPARSE_H
