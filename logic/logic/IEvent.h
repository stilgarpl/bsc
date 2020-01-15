//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_IEVENT_H
#define BASYCO_IEVENT_H

#include <core/context/Context.h>
#include <core/signal/DefaultExecutionPolicy.h>
#include <core/log/Logger.h>


template<typename idType>
class IEvent {
public:
    typedef idType IdType;
    using ExecutionPolicy = bsc::DefaultExecutionPolicy;
private:
    IdType eventId;
    bool eventValid = true;
    bsc::Context::OwnPtr eventContext;
public:
    IdType getEventId() const {
        return eventId;
    }

    void setEventId(IdType eventId) {
        IEvent::eventId = eventId;
    }


    [[nodiscard]] bool isEventValid() const {
        return eventValid;
    }

    void setEventValid(bool v) {
        eventValid = v;
    }


    IEvent();

    explicit IEvent(IdType e) : IEvent() {
        eventId = e;
    }


    [[nodiscard]] bsc::Context::Ptr context() {
        return eventContext;
    }

    [[nodiscard]] bsc::Context::Ptr context() const {
        return eventContext;
    }

};

template<typename idType>
IEvent<idType>::IEvent() : eventContext(bsc::Context::makeContext(bsc::Context::getActiveContext())) {

    eventContext->setDebugId("event context");

}

#endif //BASYCO_IEVENT_H
