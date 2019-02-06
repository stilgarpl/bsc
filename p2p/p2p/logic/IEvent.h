//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_IEVENT_H
#define BASYCO_IEVENT_H

#include <cstdlib>

#include <p2p/context/Context.h>

#include "p2p/log/Logger.h"

template<typename idType>
class IEvent {
public:
    typedef idType IdType;
    //static const bool OriginDefined = constexpr if (originType)
private:
    IdType eventId;
    bool _eventValid = true;
    mutable Context::Ptr _context;
public:
    IdType getEventId() const {
        return eventId;
    }

    void setEventId(IdType eventId) {
        IEvent::eventId = eventId;
    }


    bool isEventValid() const {
        return _eventValid;
    }

    void setEventValid(bool v) {
        _eventValid = v;
    }

    static void setup(IEvent *event) {
//        LOGGER("IEVENT SETUP");
    };


    IEvent();

    explicit IEvent(IdType e) : IEvent() {
        eventId = e;
    }


    Context::Ptr context() {
        return _context;
    }

    Context::Ptr context() const {
        return _context;
    }

};

template<typename idType>
IEvent<idType>::IEvent() {

    _context = Context::makeContext(Context::getActiveContext());
//    _context = Context::getActiveContext();

}

#endif //BASYCO_IEVENT_H
