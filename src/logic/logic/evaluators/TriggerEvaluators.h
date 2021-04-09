//
// Created by Krzysztof Tulidowicz on 30.04.19.
//

#ifndef BSC_TRIGGEREVALUATORS_H
#define BSC_TRIGGEREVALUATORS_H

#include <log/log/Logger.h>

namespace bsc {
    struct TriggerEvaluators {
        static constexpr auto triggerValue = [](auto e, auto ... args) {
            logger.debug("getting trigger value " + e.getValue());
            return e.getValue();
        };
    };
}


#endif //BSC_TRIGGEREVALUATORS_H
