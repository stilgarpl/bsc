//
// Created by Krzysztof Tulidowicz on 14.06.2021.
//
#ifndef BSC_CONCEPTS_H
#define BSC_CONCEPTS_H

#include <concepts>
#include <utility>
#include <string>

namespace bsc {
    //@todo C++20 all of those SFINAE enable_if can be replaced by concepts, but concepts do not quite work yet in
    // current
    // compilers. try again when GCC 11 comes out.
    template<typename T>
    struct IsPairT : std::false_type {};

    template<typename T, typename U>
    struct IsPairT<std::pair<T, U>> : std::true_type {};

    //    template<typename T>
    //    constexpr bool is_pair_v = is_pair<T>::value;
    template<typename T>
    concept IsPair = IsPairT<T>::value;

    template<typename T, typename _ = void>
    struct IsContainerNotStringCheck : std::false_type {};

    template<typename T>
    struct IsContainerNotStringCheck<T,
                                     std::void_t<typename T::value_type,
                                                 typename T::size_type,
                                                 typename T::allocator_type,
                                                 typename T::iterator,
                                                 typename T::const_iterator,
                                                 decltype(std::declval<T>().size()),
                                                 decltype(std::declval<T>().begin()),
                                                 decltype(std::declval<T>().end()),
                                                 decltype(std::declval<T>().cbegin()),
                                                 decltype(std::declval<T>().cend()),
                                                 std::enable_if_t<!std::is_convertible_v<T, std::string>>

                                                 >> : public std::true_type {};
    template<typename T>
    concept IsContainerNotString = IsContainerNotStringCheck<T>::value;
    /*std::negation_v<std::is_convertible<T, std::string>> && requires {
            typename T::value_type;
                    typename T::size_type;
                    typename T::allocator_type;
                    typename T::iterator;
                    typename T::const_iterator;
                    std::declval<T>().size();
                    std::declval<T>().begin();
                    std::declval<T>().end();
                    std::declval<T>().cbegin();
                    std::declval<T>().cend();
        };*/
    template<typename T>
    concept IsString = std::is_convertible_v<T, std::string>;

    template<typename T>
    concept IsEnum = std::is_enum_v<T>;

    template<typename T>
    concept IsStringView = std::is_same_v<T, std::string_view>;

    template<typename T>
    concept IsAnyString = IsString<T> || IsStringView<T>;

    template<typename T>
    concept IsClassNotString = std::is_class_v<T> && !IsString<T>;

    template<typename T>
    concept IsStringNotClass = !std::is_class_v<T> || IsString<T>;

    template <typename T>
    concept HasStdToString = requires(T t) {
        std::to_string(t);
    };

}// namespace bsc

#endif// BSC_CONCEPTS_H
