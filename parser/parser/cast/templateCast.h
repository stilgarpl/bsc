//
// Created by stilgar on 18.11.17.
//

#ifndef BSC_TEMPLATECAST_H
#define BSC_TEMPLATECAST_H

#include <utility>
#include <vector>
#include <parser/parser/fromString.h>
#include <functional>

namespace bsc {
    class IncorrectParametersException {
    public:
        std::size_t requiredParameters;
        std::size_t gotParameters;

        IncorrectParametersException(std::size_t requiredParameters, std::size_t gotParameters)
            : requiredParameters(requiredParameters), gotParameters(gotParameters){};
    };

    template<typename T, typename RetType, typename Parameter, typename... Args, typename... Strings>
    inline static RetType runStringCast(T& t, RetType (T::*f)(Args...), Strings... strings) {
        static Parser parser{};
        return (t.*f)(parser.fromString<Args>(strings)...);
    }

    template<typename T, typename RetType, typename... Args, typename... Strings>
    inline static RetType runStringCast(T& t, RetType (T::*f)(Args...), Strings... strings) {
        static Parser parser{};
        return (t.*f)(parser.fromString<Args>(strings)...);
    }

    template<typename RetType, typename... Args, typename... Strings>
    inline static RetType runStringCast(RetType (*f)(Args...), Strings... strings) {
        static Parser parser{};
        return (*f)(parser.fromString<Args>(strings)...);
    }

    template<typename RetType, typename... Args, typename... Strings>
    inline static RetType runStringCast(std::function<RetType(Args...)> f, Strings... strings) {
        static Parser parser{};
        return f(parser.fromString<Args>(strings)...);
    }

    template<size_t num_args>
    struct UnpackCaller {

    private:
        template<typename T, typename RetType, typename... Args, size_t... I>
        RetType call(T& t, RetType (T::*f)(Args...), std::vector<std::string>& args, std::index_sequence<I...>) {
            return runStringCast(t, f, args[I]...);
        }

        template<typename RetType, typename... Args, size_t... I>
        RetType call(RetType (*f)(Args...), std::vector<std::string>& args, std::index_sequence<I...>) {
            return runStringCast(f, args[I]...);
        }

        template<typename RetType, typename... Args, size_t... I>
        RetType call(std::function<RetType(Args...)> f, std::vector<std::string>& args, std::index_sequence<I...>) {
            return runStringCast(f, args[I]...);
        }

    public:
        template<typename T, typename RetType, typename... Args>
        RetType operator()(T& t, RetType (T::*f)(Args...), std::vector<std::string>& args) {
            // assert(args.size() == num_args); // just to be sure
            if (args.size() != num_args) {
                throw IncorrectParametersException(num_args, args.size());
            }
            return call(t, f, args, std::make_index_sequence<num_args>{});
        }

        template<typename RetType, typename... Args>
        RetType operator()(RetType (*f)(Args...), std::vector<std::string>& args) {
            // assert(args.size() == num_args); // just to be sure
            return call(f, args, std::make_index_sequence<num_args>{});
        }

        template<typename RetType, typename... Args>
        RetType operator()(std::function<RetType(Args...)> f, std::vector<std::string>& args) {
            // assert(args.size() == num_args); // just to be sure
            return call(f, args, std::make_index_sequence<num_args>{});
        }
    };

    template<typename T, typename RetType, typename... Args>
    inline RetType runMemberFunction(T& t, RetType (T::*f)(Args...), std::vector<std::string> values) {
        bsc::UnpackCaller<sizeof...(Args)> a;
        return a(t, f, values);
    }

    template<typename RetType, typename... Args>
    inline RetType runFunction(RetType (*f)(Args...), std::vector<std::string> values) {
        bsc::UnpackCaller<sizeof...(Args)> a;
        return a(f, values);
    }

    template<typename RetType, typename... Args>
    inline RetType runStandardFunction(std::function<RetType(Args...)> f, std::vector<std::string> values) {
        bsc::UnpackCaller<sizeof...(Args)> a;
        return a(f, values);
    }
}


#endif //BSC_TEMPLATECAST_H
