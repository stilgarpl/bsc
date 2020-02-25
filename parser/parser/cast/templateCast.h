//
// Created by stilgar on 18.11.17.
//

#ifndef BSC_TEMPLATECAST_H
#define BSC_TEMPLATECAST_H

#include <cstddef>
#include <utility>
#include <vector>
#include <parser/parser/fromString.h>
//#include "from_string.h"

namespace bsc {
    class IncorrectParametersException {
    public:
        size_t requiredParameters;
        size_t gotParameters;

        IncorrectParametersException(size_t requiredParameters, size_t gotParameters) : requiredParameters(
                requiredParameters), gotParameters(gotParameters) {};


    };

    template<typename T, typename RetType, typename Parameter, typename ... Args, typename ... Strings>
    static void runStringCast(T& t, RetType (T::*f)(Args...), Strings ... strings) {
        static Parser parser{};
        (t.*f)(parser.fromString<Args>(strings)...);
    }

    template<typename T, typename RetType, typename ... Args, typename ... Strings>
    static void runStringCast(T& t, RetType (T::*f)(Args...), Strings ... strings) {
        static Parser parser{};
        (t.*f)(parser.fromString<Args>(strings)...);
    }

    template<typename RetType, typename ... Args, typename ... Strings>
    static void runStringCast(RetType (* f)(Args...), Strings ... strings) {
        static Parser parser{};
        (*f)(parser.fromString<Args>(strings)...);
    }

    template<typename RetType, typename ... Args, typename ... Strings>
    static void runStringCast(std::function<RetType(Args...)> f, Strings ... strings) {
        static Parser parser{};
        f(parser.fromString<Args>(strings)...);
    }

    template<size_t num_args>
    struct UnpackCaller {

    private:

        template<typename T, typename RetType, typename ... Args, size_t... I>
        void call(T& t, RetType(T::*f)(Args...), std::vector<std::string>& args, std::index_sequence<I...>) {
            bsc::runStringCast(t, f, args[I]...);
        }

        template<typename RetType, typename ... Args, size_t... I>
        void call(RetType(* f)(Args...), std::vector<std::string>& args, std::index_sequence<I...>) {
            (*f)(args[I]...);
        }

        template<typename RetType, typename ... Args, size_t... I>
        void call(std::function<RetType(Args...)> f, std::vector<std::string>& args, std::index_sequence<I...>) {
            runStringCast(f, args[I]...);
        }


    public:
        template<typename T, typename RetType, typename ... Args>
        void operator()(T& t, RetType(T::*f)(Args...), std::vector<std::string>& args) {
            //assert(args.size() == num_args); // just to be sure
            if (args.size() != num_args) {
                throw IncorrectParametersException(num_args, args.size());
            }
            call(t, f, args, std::make_index_sequence<num_args>{});
        }

        template<typename RetType, typename ... Args>
        void operator()(RetType(* f)(Args...), std::vector<std::string>& args) {
            //assert(args.size() == num_args); // just to be sure
            call(f, args, std::make_index_sequence<num_args>{});
        }

        template<typename RetType, typename ... Args>
        void operator()(std::function<RetType(Args...)> f, std::vector<std::string>& args) {
            //assert(args.size() == num_args); // just to be sure
            call(f, args, std::make_index_sequence<num_args>{});
        }
    };

    template<typename T, typename RetType, typename ... Args>
    void runMemberFunction(T& t, RetType (T::*f)(Args...), std::vector<std::string> values) {
        bsc::UnpackCaller<sizeof... (Args)> a;
        a(t, f, values);
    }

    template<typename RetType, typename ... Args>
    void runFunction(RetType (* f)(Args...), std::vector<std::string> values) {
        bsc::UnpackCaller<sizeof... (Args)> a;
        a(f, values);
    }

    template<typename RetType, typename ... Args>
    void runStandardFunction(std::function<RetType(Args...)> f, std::vector<std::string> values) {
        bsc::UnpackCaller<sizeof... (Args)> a;
        a(f, values);
    }
}


#endif //BSC_TEMPLATECAST_H
