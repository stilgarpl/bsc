//
// Created by stilgar on 18.11.17.
//

#ifndef BASYCO_TEMPLATE_CAST_H
#define BASYCO_TEMPLATE_CAST_H

#include <cstddef>
#include <utility>
#include <vector>
#include "from_string.h"

class IncorrectParametersException {
public:
    size_t requiredParameters;
    size_t gotParameters;

    IncorrectParametersException(size_t requiredParameters, size_t gotParameters) : requiredParameters(
            requiredParameters), gotParameters(gotParameters) {};


};

template<typename T, typename RetType, typename ... Args, typename ... Strings>
static void runStringCast(T &t, RetType (T::*f)(Args...), Strings ... strings) {
    (t.*f)(fromString<Args>(strings)...);
}


template<typename RetType, typename ... Args, typename ... Strings>
static void runStringCast(RetType (*f)(Args...), Strings ... strings) {
    (*f)(fromString<Args>(strings)...);
}


template<size_t num_args>
struct UnpackCaller {

private:
    template<typename T, typename RetType, typename ... Args, size_t... I>
    void call(T& t, RetType(T::*f)(Args...), std::vector<std::string>& args, std::index_sequence<I...>) {
        runStringCast(t, f, args[I]...);
    }

    template<typename RetType, typename ... Args, size_t... I>
    void call(RetType(* f)(Args...), std::vector<std::string>& args, std::index_sequence<I...>) {
        (*f)(args[I]...);
    }


public:
    template<typename T, typename RetType, typename ... Args>
    void operator()(T &t, RetType(T::*f)(Args...), std::vector<std::string> &args) {
        //assert(args.size() == num_args); // just to be sure
        if (args.size() != num_args) {
            throw IncorrectParametersException(num_args, args.size());
        }
        call(t, f, args, std::make_index_sequence<num_args>{});
    }

    template<typename RetType, typename ... Args>
    void operator()(RetType(*f)(Args...), std::vector<std::string> &args) {
        //assert(args.size() == num_args); // just to be sure
        call(f, args, std::make_index_sequence<num_args>{});
    }
};


template<typename T, typename RetType, typename ... Args>
void runMemberFunction(T &t, RetType (T::*f)(Args...), std::vector<std::string> values) {
    UnpackCaller<sizeof... (Args)> a;
    a(t, f, values);
}

template<typename RetType, typename ... Args>
void runFunction(RetType (*f)(Args...), std::vector<std::string> values) {
    UnpackCaller<sizeof... (Args)> a;
    a(f, values);
}


#endif //BASYCO_TEMPLATE_CAST_H
