//
// Created by stilgar on 2/1/19.
//

#ifndef BASYCO_COMMONEVALUATORS_H
#define BASYCO_COMMONEVALUATORS_H


#include <p2p/log/Logger.h>
#include <p2p/context/Context.h>
#include <p2p/logic/common/ForeachContext.h>

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
            return Context::getActiveContext()->getSafe<ForeachContext<T>>()->getValue();
        };
    }




};


#endif //BASYCO_COMMONEVALUATORS_H
