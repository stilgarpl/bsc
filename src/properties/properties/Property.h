//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//

#ifndef BSC_PROPERTY_H
#define BSC_PROPERTY_H

#include "PropertyContext.h"
#include "PropertyDefinitions.h"
#include <algorithm>
#include <optional>
#include <parser/parser/explode.h>

namespace bsc {

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

        class DirectPropertyMapper {};

        template<typename T>
        concept IsDirect = (std::is_same_v<std::remove_cvref_t<T>, detail::DirectPropertyMapper>);
        template<typename T>
        concept IsNotDirect = (!IsDirect<T>);
    }// namespace detail

    template<detail::StringLiteral lit, typename T = detail::DirectPropertyMapper>
    class Property {
        //@todo C++20, constexpr
        //@todo C++20 not possible yet, stdlibc++ does not have constexpr std::string
        const PropertyIdType propertyId     = {lit.value};
        const PropertyIdSequence idSequence = explode(propertyId, propertyDelimiter);
        //@todo constexpr split propertyId by . into std::array<std::string>, pass that to get<T> - it should iterate over that array with
        //[] and return .as<T> when it gets to the end
    public:
        constexpr auto getPropertyName() { return propertyId; }

        constexpr auto getValue() requires detail::IsNotDirect<T> {
            return Context::getActiveContext()->get<PropertyContext>()->getPropertyData()->get<T>(idSequence);
        }

        template<typename TrueT>
        constexpr TrueT getValue() requires detail::IsDirect<T> {
            return Context::getActiveContext()->get<PropertyContext>()->getPropertyData()->get<TrueT>(idSequence);
        }
    };

}// namespace bsc

#endif// BSC_PROPERTY_H
