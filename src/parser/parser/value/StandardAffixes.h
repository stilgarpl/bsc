//
// Created by Krzysztof Tulidowicz on 01.02.2022.
//
#ifndef BSC_STANDARDAFFIXES_H
#define BSC_STANDARDAFFIXES_H

#include "Affix.h"
#include <magic_enum.hpp>
#include <ranges>
#include <string>

namespace bsc {

    namespace affixes {

        template<typename AffixType, typename T>
        concept IsRelationalAffix = Affix<AffixType> && requires(AffixType a, T t, T k) {
            { a.compare(t, k) } -> std::same_as<bool>;
        };

        template<typename AffixType, typename T>
        concept IsValueTransformAffix = Affix<AffixType> && requires(AffixType a, T t, typename AffixType::ValueType k) {
            a.transform(t, k);
        };

        template<typename AffixType>
        concept IsValueAffix = Affix<AffixType> && requires(AffixType a) {
            a.value;
        };

        template<typename valueType>
        struct BaseAffix {
            using ValueType = valueType;
            std::optional<ValueType> value;
        };

        template<>
        struct BaseAffix<void> {
            using ValueType = void;
        };

        struct NoAffix : BaseAffix<void> {
            const bool present  = false;
            const bool optional = true;
            static inline const std::string pattern;
        };

        struct SignAffix : BaseAffix<std::string> {
            const bool present                      = true;
            const bool optional                     = true;
            static inline const std::string pattern = "[+-]";
            template<typename T>
            T transform(const T& t) {
                if (value == "-") {
                    return -t;
                } else {
                    return t;
                }
            }
        };

        template<typename EnumType, bool Optional = true>
        requires std::is_enum_v<EnumType>
        struct EnumAffix : BaseAffix<EnumType> {
            const bool present  = true;
            const bool optional = Optional;

        private:
            static auto constructPattern() {
                //@todo C++20 GCC12 make this constexpr
                using namespace std::string_literals;
                std::string result = "(";
                for (bool first = true; const auto& name : magic_enum::enum_names<EnumType>()) {
                    result += (first ? ""s : "|"s) + std::string(name);
                    first = false;
                }
                result += ")";
                return result;
            }

        public:
            static inline const std::string pattern = constructPattern();
        };

        template<Affix AffixType, auto transformator>
        struct TransformAffix : public AffixType {
            template<typename T>
            auto transform(const T& t) {
                return transformator(t, *this->value);
            }
        };

    }// namespace affixes

}// namespace bsc

#endif// BSC_STANDARDAFFIXES_H
