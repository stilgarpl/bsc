//
// Created by stilgar on 08.01.2020.
//

#ifndef BASYCO_PARAMETERPARSE_H
#define BASYCO_PARAMETERPARSE_H

#include <type_traits>
#include <limits>
#include <string>
#include <stdexcept>

class ParameterParseException : public std::invalid_argument {
public:
    explicit ParameterParseException(const std::string& arg) : invalid_argument(arg) {}
};

template<typename ParameterType>
std::enable_if_t<std::numeric_limits<ParameterType>::is_integer, ParameterType> parse(const char* value) {
    //@todo stoi, stol, stoii based on Parameter type length?
    try {
        return std::stol(value);
    } catch (std::invalid_argument& e) {
        throw ParameterParseException("Integer parsing failed.");
    }
}

template<typename ParameterType>
std::enable_if_t<std::is_convertible_v<ParameterType, std::string>, ParameterType> parse(const char* value) {
    if (value != nullptr) {
        return ParameterType(value);
    } else {
        throw ParameterParseException("Can't parse null parameter value");
    }

}

template<>
inline bool parse<bool>(const char* value) {
    return true;
}


#endif //BASYCO_PARAMETERPARSE_H
