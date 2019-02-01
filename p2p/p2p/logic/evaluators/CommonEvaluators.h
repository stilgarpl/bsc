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

};


#endif //BASYCO_COMMONEVALUATORS_H
