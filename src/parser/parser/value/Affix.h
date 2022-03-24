//
// Created by Krzysztof Tulidowicz on 24.03.2022.
//
#ifndef BSC_AFFIX_H
#define BSC_AFFIX_H
#include <concepts>
#include <string>

namespace bsc {
    template<typename T>
    concept Affix = requires(T) {
        { T::present } -> std::convertible_to<bool>;
        { T::optional } -> std::convertible_to<bool>;
        { T::pattern } -> std::convertible_to<std::string>;
        typename T::ValueType;
    };
}

#endif// BSC_AFFIX_H
