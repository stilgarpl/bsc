//
// Created by stilgar on 05.02.19.
//

#ifndef BSC_TIMECONDITIONS_H
#define BSC_TIMECONDITIONS_H


#include "../LogicObject.h"


namespace bsc {
    struct TimeConditions {

        //@todo add offset to event tick, for example every 60s + 15s offset so not every event is at the same moment
        static auto every(typename Tick::IdType id) {
            return LogicObject::event<Tick>(id);
        }

    };
}


#endif //BSC_TIMECONDITIONS_H
