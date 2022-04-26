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

        struct RelationAffix : public BaseAffix<std::string> {
            const bool present                      = true;
            const bool optional                     = true;
            static inline const std::string pattern = "(?:<|<=|=|>|>=)";

        private:
            bool comparator(std::string_view operand, auto t1, auto t2) const{
                if (operand == "<") {
                    return t1 < t2;
                } else if (operand == "<=") {
                    return t1 <= t2;
                } else if (operand == ">") {
                    return t1 > t2;
                } else if (operand == ">=") {
                    return t1 >= t2;
                } else if (operand == "=") {
                    return t1 == t2;
                } else if (operand == "!=") {
                    return t1 != t2;
                }

                return t1 == t2;// default operation is equality
            }

        public:
            bool compare(const auto& t, const auto& k) const {
                //@todo C++23 use monadic interface
                return comparator(value.has_value() ? *value : "", t, k);
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
                std::string result = "(?:";
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

    }// namespace affixes

}// namespace bsc

#endif// BSC_STANDARDAFFIXES_H
