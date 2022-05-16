//
// Created by Krzysztof Tulidowicz on 27.08.2021.
//
#ifndef BSC_OPTIONAL_H
#define BSC_OPTIONAL_H

#include <optional>

namespace bsc::optional {

    template<typename T>
    concept IsOptional = std::same_as < std::remove_cvref_t<T>,
    std::optional < typename std::remove_cvref_t<T>::value_type >> ;

    template<typename T, typename Callable>
    auto operator|(const std::optional<T>& opt, Callable callable) {
        return callable(opt);
    }

    template<typename Callable>
    auto ifPresent(Callable callable) {
        return [callable](IsOptional auto& optional) {
            if (optional.has_value()) {
                callable(*optional);
            }
        };
    }

    template<typename Predicate>
    auto filter(Predicate predicate) {
        return [predicate](IsOptional auto& optional) {
            if (optional.has_value() && predicate(*optional)) {
                return optional;
            }
            return decltype(optional){std::nullopt};
        };
    }

    template<typename Callable>
    auto map(Callable callable) {
        return [callable](IsOptional auto& optional) {
            using RetType = std::invoke_result_t<Callable, typename std::remove_cvref_t<decltype(optional)>::value_type>;
            if (optional.has_value()) {
                return std::optional<RetType>(callable(*optional));
            } else {
                return std::optional<RetType>();
            };
        };
    }

    template<typename Callable>
    auto flatMap(Callable callable) {
        return [callable](IsOptional auto& optional) {
            using RetType = std::invoke_result_t<Callable, typename std::remove_cvref_t<decltype(optional)>::value_type>;
            if (optional.has_value()) {
                return callable(*optional);
            } else {
                return RetType{};
            };
        };
    }

    template<typename Other>
    auto orElse(Other value) {
        return [value](IsOptional auto& optional) {
            if (optional.has_value()) {
                return *optional;
            } else {
                return value;
            };
        };
    }

    template<typename Callable>
    auto orElseGet(Callable callable) {
        return [callable](IsOptional auto& optional) {
            if (optional.has_value()) {
                return *optional;
            } else {
                return callable();
            };
        };
    }

    template<typename Callable>
    auto orElseChainGet(Callable callable) {
        return [callable](IsOptional auto& optional) {
            if (optional.has_value()) {
                return optional;
            } else {
                return callable();
            };
        };
    }

    template<typename Exception>
    auto orElseThrow(Exception exception) {
        return [exception](IsOptional auto& optional) {
            if (optional.has_value()) {
                return *optional;
            } else {
                throw exception;
            };
        };
    }

    template<typename Nullable>
    auto ofNullable(Nullable&& nullable) {
        if (nullable == nullptr) {
            return std::optional<std::remove_cvref_t<Nullable>>(std::nullopt);
        } else {
            return std::optional<std::remove_cvref_t<Nullable>>(std::forward<Nullable>(nullable));
        }
    }

    template<typename T>
    auto of(T t) requires (!IsOptional<T>) {
        return std::optional(t);
    }

    auto ofThrowable(auto callable, auto... args) {
        using ReturnType = std::invoke_result_t<decltype(callable), decltype(args)...>;
        try {
            if constexpr (IsOptional<ReturnType>) {
                return callable(args...);
            } else {
                return std::optional(callable(args...));
            }
        } catch (...) {
            return std::optional<ReturnType >{std::nullopt};
        }
    }
}// namespace bsc::optional

#endif// BSC_OPTIONAL_H
