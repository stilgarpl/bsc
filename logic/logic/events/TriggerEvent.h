//
// Created by stilgar on 24.03.19.
//

#ifndef BASYCO_TRIGGEREVENT_H
#define BASYCO_TRIGGEREVENT_H

#include "../IEvent.h"


namespace bsc {
    template<typename TriggerIdType, typename TriggerValueType = void>
    class TriggerEvent : public bsc::IEvent<TriggerIdType> {

    private:
        TriggerValueType value;

    public:

        explicit TriggerEvent(TriggerValueType value) : value(value) {}

        TriggerEvent(TriggerIdType e, TriggerValueType value) : IEvent<TriggerIdType>(e), value(value) {}

        TriggerValueType getValue() const {
            return value;
        }

    };

    template<typename TriggerIdType>
    class TriggerEvent<TriggerIdType, void> : public bsc::IEvent<TriggerIdType> {

    public:
        explicit TriggerEvent(TriggerIdType e) : IEvent<TriggerIdType>(e) {}

        TriggerEvent() = default;

    public:

    };
}


#endif //BASYCO_TRIGGEREVENT_H
