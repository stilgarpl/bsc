//
// Created by stilgar on 30.01.19.
//

#ifndef BASYCO_EVENTWRAPPER_H
#define BASYCO_EVENTWRAPPER_H


#include <p2p/logic/IEvent.h>

template<typename Type>
class EventWrapper : public IEvent<int> {
public:
    using PayloadType = Type;
private:
    Type payload;

public:
    explicit EventWrapper(Type payload) : payload(payload) {}

    Type &getPayload() {
        return payload;
    }

    operator Type &() {
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

template<typename Type>
class ConditionalEventWrapper : public EventWrapper<Type> {
private:
    Type otherValue;
public:
    ConditionalEventWrapper(Type payload, Type otherValue) : EventWrapper<Type>(payload), otherValue(otherValue) {}

public:
    const Type &getOtherValue() const {
        return otherValue;
    }
};


#endif //BASYCO_EVENTWRAPPER_H