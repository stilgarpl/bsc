//
// Created by stilgar on 24.03.19.
//

#ifndef BASYCO_TRIGGERCONDITIONS_H
#define BASYCO_TRIGGERCONDITIONS_H


#include <p2p/logic/LogicObject.h>
#include <p2p/logic/events/TriggerEvent.h>

struct TriggerConditions {

    template<typename TriggerIdType>
    static auto trigger(TriggerIdType triggerId) {
        return LogicObject::event<TriggerEvent<TriggerIdType>>(triggerId);
    }

    template<typename TriggerIdType, typename TriggerValue>
    static auto trigger(TriggerIdType triggerId) {
        return LogicObject::event<TriggerEvent<TriggerIdType, TriggerValue>>(triggerId);
    }

};


#endif //BASYCO_TRIGGERCONDITIONS_H
