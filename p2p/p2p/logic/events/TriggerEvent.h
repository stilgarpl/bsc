//
// Created by stilgar on 24.03.19.
//

#ifndef BASYCO_TRIGGEREVENT_H
#define BASYCO_TRIGGEREVENT_H

#include <p2p/logic/IEvent.h>

template<typename TriggerIdType, typename TriggerValueType = void>
class TriggerEvent : public IEvent<TriggerIdType> {

private:
    TriggerValueType value;

public:

    explicit TriggerEvent(TriggerValueType value) : value(value) {}

    TriggerEvent(TriggerIdType e, TriggerValueType value) : IEvent<TriggerIdType>(e), value(value) {}

};


template<typename TriggerIdType>
class TriggerEvent<TriggerIdType, void> : public IEvent<TriggerIdType> {

public:
    explicit TriggerEvent(TriggerIdType e) : IEvent<TriggerIdType>(e) {}

    TriggerEvent() = default;

public:

};


#endif //BASYCO_TRIGGEREVENT_H
