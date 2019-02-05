//
// Created by stilgar on 05.02.19.
//

#ifndef BASYCO_TIMECONDITIONS_H
#define BASYCO_TIMECONDITIONS_H


#include <p2p/logic/LogicObject.h>

struct TimeConditions {


    static auto every(typename Tick::IdType id) {
        return LogicObject::EventHelper<Tick>(id);
    }

};


#endif //BASYCO_TIMECONDITIONS_H
