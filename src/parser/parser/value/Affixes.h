//
// Created by Krzysztof Tulidowicz on 01.02.2022.
//
#ifndef BSC_AFFIXES_H
#define BSC_AFFIXES_H

#include <magic_enum.hpp>
#include <ranges>
#include <string>
namespace bsc {

    namespace affixes {
        struct NoAffix {
            const bool present  = false;
            const bool optional = true;
            static inline const std::string pattern;
            using ValueType = void;
        };

        constexpr static const inline NoAffix noAffix{};

        struct SignAffix {
            const bool present  = true;
            const bool optional = true;
            static inline const std::string pattern="[+-]";
            using ValueType = std::string;
        };

        constexpr static const inline SignAffix signAffix{};

        template<typename EnumType ,bool Optional = true> requires std::is_enum_v<EnumType>
        struct EnumAffix {
            const bool present  = true;
            const bool optional = Optional;
            using ValueType = EnumType;
            static auto constructPattern() {
                //@todo C++20 GCC12 make this constexpr
                using namespace std::string_literals;
                std::string result = "(";
                for (bool first = true; const auto& name : magic_enum::enum_names<EnumType>()) {
                    result += (first ? ""s : "|"s)  + std::string(name);
                    first = false;
                }
                result += ")";
                return result;
            }
            static inline const std::string pattern= constructPattern();
        };

    }


}


#endif// BSC_AFFIXES_H
