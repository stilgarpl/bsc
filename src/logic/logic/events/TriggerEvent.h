//
// Created by Krzysztof Tulidowicz on 24.03.19.
//

#ifndef BSC_TRIGGEREVENT_H
#define BSC_TRIGGEREVENT_H

#include <logic/IEvent.h>

namespace bsc {
    template<typename TriggerIdType, typename TriggerValueType = void>
    class TriggerEvent : public IEvent<TriggerIdType> {

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


#endif //BSC_TRIGGEREVENT_H
