//
// Created by Krzysztof Tulidowicz on 24.03.2022.
//
#ifndef BSC_AFFIX_H
#define BSC_AFFIX_H
#include <concepts>
#include <exception>
#include <optional>
#include <stdexcept>
#include <string>
namespace bsc::affixes {
    template<typename T>
        concept IsAffix = requires(T) {
                            { T::present } -> std::convertible_to<bool>;
                            { T::optional } -> std::convertible_to<bool>;
                            { T::pattern } -> std::convertible_to<std::string>; //IMPORTANT, pattern can't have capturing groups!
                            typename T::ValueType;
                        };

        class EmptyAffixException : public std::out_of_range {
        public:
            explicit EmptyAffixException(const std::string& arg) : out_of_range(arg) {
            }
        };

        template<typename AffixType, typename T>
        concept IsRelationalAffix = IsAffix<AffixType> && requires(AffixType a, T t) {
                                                            { a.compare(t, t) } -> std::same_as<bool>;
                                                        };

        template<typename AffixType, typename T>
        concept IsValueTransformAffix = IsAffix<AffixType> && requires(AffixType a, T const& t) { a.transform(t); };

        template<typename AffixType>
        concept IsValueAffix = IsAffix<AffixType> && requires(AffixType a) { a.value; };

        template<typename valueType>
        struct BaseAffix {
            using ValueType = valueType;
            std::optional<ValueType> value;
        };

        template<>
        struct BaseAffix<void> {
            using ValueType = void;
        };

}

#endif// BSC_AFFIX_H
