//
// Created by Krzysztof Tulidowicz on 18.11.17.
//

#ifndef BSC_TEMPLATECAST_H
#define BSC_TEMPLATECAST_H

#include <functional>
#include <parser/parser/fromString.h>
#include <span>
#include <utility>
#include <vector>

namespace bsc {
    class IncorrectParametersCountException {
    public:
        std::size_t requiredParameters;
        std::size_t gotParameters;

        IncorrectParametersCountException(std::size_t requiredParameters, std::size_t gotParameters)
            : requiredParameters(requiredParameters), gotParameters(gotParameters){};
    };

    namespace detail {

        template<typename T, typename RetType, typename Parameter, typename... Args, typename... Strings>
        inline static RetType runStringCast(T& t, RetType (T::*f)(Args...), Strings&&... strings) {
            return (t.*f)(fromString<Args>(std::forward<Strings>(strings))...);
        }

        template<typename T, typename RetType, typename... Args, typename... Strings>
        inline static RetType runStringCast(T& t, RetType (T::*f)(Args...), Strings&&... strings) {
            return (t.*f)(fromString<Args>(std::forward<Strings>(strings))...);
        }

        template<typename RetType, typename... Args, typename... Strings>
        inline static RetType runStringCast(RetType (*f)(Args...), Strings&&... strings) {
            return (*f)(fromString<Args>(std::forward<Strings>(strings))...);
        }

        template<typename RetType, typename... Args, typename... Strings>
        inline static RetType runStringCast(std::function<RetType(Args...)> f, Strings&&... strings) {
            return f(fromString<Args>(std::forward<Strings>(strings))...);
        }

        template<size_t num_args>
        struct UnpackCaller {

        private:
            template<typename T, typename RetType, typename... Args, size_t... I>
            RetType call(T& t, RetType (T::*f)(Args...), const std::span<const std::string>& args, std::index_sequence<I...>) {
                return runStringCast(t, f, args[I]...);
            }

            template<typename RetType, typename... Args, size_t... I>
            RetType call(RetType (*f)(Args...), const std::span<const std::string>& args, std::index_sequence<I...>) {
                return runStringCast(f, args[I]...);
            }

            template<typename RetType, typename... Args, size_t... I>
            RetType call(std::function<RetType(Args...)> f, const std::span<const std::string>& args, std::index_sequence<I...>) {
                return runStringCast(f, args[I]...);
            }

        public:
            template<typename T, typename RetType, typename... Args>
            RetType operator()(T& t, RetType (T::*f)(Args...), std::span<const std::string> args) {
                if (args.size() != num_args) {
                    throw IncorrectParametersCountException(num_args, args.size());
                }
                return call(t, f, args, std::make_index_sequence<num_args>{});
            }

            template<typename RetType, typename... Args>
            RetType operator()(RetType (*f)(Args...), std::span<const std::string> args) {
                if (args.size() != num_args) {
                    throw IncorrectParametersCountException(num_args, args.size());
                }
                return call(f, args, std::make_index_sequence<num_args>{});
            }

            template<typename RetType, typename... Args>
            RetType operator()(std::function<RetType(Args...)> f, std::span<const std::string> args) {
                if (args.size() != num_args) {
                    throw IncorrectParametersCountException(num_args, args.size());
                }
                return call(f, args, std::make_index_sequence<num_args>{});
            }
        };
    }// namespace detail

    template<typename T, typename RetType, typename... Args>
    inline RetType runMemberFunction(T& t, RetType (T::*f)(Args...), const std::vector<std::string>& values) {
        detail::UnpackCaller<sizeof...(Args)> a;
        std::span spanValues = values;
        return a(t, f, spanValues);
    }

    template<typename RetType, typename... Args>
    inline RetType runFunction(RetType (*f)(Args...), const std::vector<std::string>& values) {
        detail::UnpackCaller<sizeof...(Args)> a;
        std::span spanValues = values;
        return a(f, spanValues);
    }

    template<typename RetType, typename... Args>
    inline RetType runStandardFunction(std::function<RetType(Args...)> f, const std::vector<std::string>& values) {
        detail::UnpackCaller<sizeof...(Args)> a;
        std::span spanValues = values;
        return a(f, spanValues);
    }

    template<typename T, typename RetType, typename... Args>
    inline RetType runMemberFunction(T& t, RetType (T::*f)(Args...), std::span<std::string> values) {
        detail::UnpackCaller<sizeof...(Args)> a;
        return a(t, f, values);
    }

    template<typename RetType, typename... Args>
    inline RetType runFunction(RetType (*f)(Args...), std::span<std::string> values) {
        detail::UnpackCaller<sizeof...(Args)> a;
        return a(f, values);
    }

    template<typename RetType, typename... Args>
    inline RetType runStandardFunction(std::function<RetType(Args...)> f, std::span<std::string> values) {
        detail::UnpackCaller<sizeof...(Args)> a;
        return a(f, values);
    }
}


#endif //BSC_TEMPLATECAST_H
