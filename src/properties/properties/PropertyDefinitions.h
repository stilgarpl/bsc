//
// Created by Krzysztof Tulidowicz on 21.01.2021.
//

#ifndef BSC_PROPERTYDEFINITIONS_H
#define BSC_PROPERTYDEFINITIONS_H
#include <filesystem>
#include <string>
#include <vector>

namespace bsc {
    namespace fs             = std::filesystem;
    using PropertyIdType     = std::string;
    using PropertyIdSequence = std::vector<PropertyIdType>;
    using PropertyValueType  = std::string;

    constexpr char propertyDelimiter = '.';

    namespace detail {
        // taken from https://ctrpeach.io/posts/cpp20-string-literal-template-parameters/
        template<size_t N>
        struct StringLiteral {
            //@todo C++20 should remove this class when std::string is constexpr
            constexpr StringLiteral(const char (&str)[N]) { std::copy_n(str, N, value); }

            char value[N];
            auto operator<=>(const StringLiteral&) const = default;
            bool operator==(const StringLiteral&) const  = default;
        };

        struct DirectPropertyMapper {
            std::string value;
        };

        template<typename T>
        concept IsDirect = (std::is_same_v<std::remove_cvref_t<T>, detail::DirectPropertyMapper>);
        template<typename T>
        concept IsNotDirect = (!IsDirect<T>);
    }// namespace detail
}// namespace bsc

#endif// BSC_PROPERTYDEFINITIONS_H
