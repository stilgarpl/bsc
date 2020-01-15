//
// Created by stilgar on 30.01.19.
//

#ifndef BASYCO_EVENTWRAPPER_H
#define BASYCO_EVENTWRAPPER_H


#include "../IEvent.h"


namespace bsc {
    template<typename Type>
    class EventWrapper : public IEvent<int> {
    public:
        using PayloadType = Type;
    private:
        Type payload;

    public:
        explicit EventWrapper(Type payload) : payload(payload) {}

        Type& getPayload() {
            return payload;
        }

        operator Type&() {
            return payload;
        }

        operator Type() {
            return payload;
        }
    };

    template<>
    struct EventWrapper<void> : public IEvent<int> {

    public:
        template<typename ... T>
        explicit EventWrapper(T... t) {}
    };

    template<typename EventType, typename ValueType>
    class ConditionalEventWrapper : public EventWrapper<EventType> {
    private:
        ValueType otherValue;
    public:
        ConditionalEventWrapper(EventType payload, ValueType otherValue) : EventWrapper<EventType>(payload),
                                                                           otherValue(otherValue) {}

    public:
        const ValueType& getOtherValue() const {
            return otherValue;
        }
    };
}


#endif //BASYCO_EVENTWRAPPER_H