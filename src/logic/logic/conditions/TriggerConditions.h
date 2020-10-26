//
// Created by Krzysztof Tulidowicz on 24.03.19.
//

#ifndef BSC_TRIGGERCONDITIONS_H
#define BSC_TRIGGERCONDITIONS_H

#include <logic/events/TriggerEvent.h>
namespace bsc {
    struct TriggerConditions {

        template<typename TriggerIdType>
        static auto trigger(TriggerIdType triggerId) {
            return LogicObject::event < TriggerEvent < TriggerIdType >> (triggerId);
        }

        template<typename TriggerIdType, typename TriggerValue>
        static auto trigger(TriggerIdType triggerId) {
            return LogicObject::event < TriggerEvent < TriggerIdType, TriggerValue >> (triggerId);
        }

    };
}


#endif //BSC_TRIGGERCONDITIONS_H
