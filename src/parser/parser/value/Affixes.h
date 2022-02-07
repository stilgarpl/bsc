//
// Created by Krzysztof Tulidowicz on 01.02.2022.
//
#ifndef BSC_AFFIXES_H
#define BSC_AFFIXES_H

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

        //@todo move to other file
        enum class SiPrefix {
            y = -24,
            z = -21,
            a = -18,
            f = -15,
            p = -12,
            n = -9,
            u = -6,
            m = -3,
            c = -2,
            d = -1,
            da = 1,
            h = 2,
            k = 3,
            M = 6,
            G = 9,
            T = 12,
            P = 15,
            E = 18,
            Z = 21,
            Y = 24,
        };

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
