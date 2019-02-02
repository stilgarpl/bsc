//
// Created by stilgar on 2/1/19.
//

#ifndef BASYCO_COMMONEVALUATORS_H
#define BASYCO_COMMONEVALUATORS_H


#include <p2p/log/Logger.h>

struct CommonEvaluators {

    static constexpr auto unwrapEvent = [](auto e) {
        LOGGER("unwrapping event")
        return e.getPayload();
    };

    static constexpr auto passthrough = [](auto e) {
        LOGGER("forwarding event")
        return e;
    };

    template<typename T>
    static auto variable(T var) {
        return [var](auto e) {
            return var;
        };
    }

//    static auto unwrapEvent();

};


#endif //BASYCO_COMMONEVALUATORS_H
