//
// Created by stilgar on 30.04.19.
//

#ifndef BASYCO_TRIGGEREVALUATORS_H
#define BASYCO_TRIGGEREVALUATORS_H


#include <core/log/Logger.h>


namespace bsc {
    struct TriggerEvaluators {
        static constexpr auto triggerValue = [](auto e, auto ... args) {
            LOGGER("getting trigger value " + e.getValue())
            return e.getValue();
        };
    };
}


#endif //BASYCO_TRIGGEREVALUATORS_H
