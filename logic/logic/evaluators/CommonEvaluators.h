//
// Created by stilgar on 2/1/19.
//

#ifndef BASYCO_COMMONEVALUATORS_H
#define BASYCO_COMMONEVALUATORS_H


#include <core/log/Logger.h>
#include <core/context/Context.h>
#include "../common/ForeachContext.h"

struct CommonEvaluators {

    static constexpr auto unwrapEvent = [](auto e, auto ... args) {
        LOGGER("unwrapping event")
        return e.getPayload();
    };

    static constexpr auto passthrough = [](auto e, auto ... args) {
        LOGGER("forwarding event")
        return e;
    };

    template<typename T>
    static auto value(T var) {
        return [var](auto e, auto ... args) {
            return var;
        };
    }

    //@todo add override of foreach with parameter that transforms value to something else.
    template<typename T>
    static auto foreachValue() {
        return [](auto e, auto ... args) -> T & {
            //@todo error handling
            //@todo ^^ I made a fix for that, each event have separate context. should fix this, but... think about it.
            return Context::getActiveContext()->getSafe<ForeachContext<T>>().getValue();
        };
    }

    //@todo I removed additional args for this evaluator. I'm not sure anything actually uses additional arguments. Maybe I should remove that ability.
    template<typename ... Evaluators>
    static auto stack(Evaluators... evaluators) {
        return [evaluators...](auto e) {
            return evaluateStack(e, evaluators...);

        };
    }

private:
    template<typename E,  typename Evaluator, typename ...RestOfEvaluators>
    constexpr static auto
    evaluateStack(const E &e, Evaluator evaluator, RestOfEvaluators ... rest) {
        //@todo maybe use fold expression instead of recursion? that would probably allow to use args... again!
        if constexpr (sizeof... (rest) == 0) {
            return evaluator(e);
        } else {
            return evaluator(evaluateStack(e, rest...));
        }
    }
};


#endif //BASYCO_COMMONEVALUATORS_H
