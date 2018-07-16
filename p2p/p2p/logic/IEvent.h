//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_IEVENT_H
#define BASYCO_IEVENT_H

#include <cstdlib>

#include <p2p/context/Context.h>

#include "p2p/log/Logger.h"

typedef std::nullptr_t NullOrigin;

template<typename OriginType>
struct setupOrigin {
    static OriginType &setup(OriginType &origin) {
        //@todo log some error of unimplemented origin setup

        return origin;
    }
};


template<>
struct setupOrigin<NullOrigin> {

    static NullOrigin &setup(NullOrigin &origin) {
        //null origin means no origin is set, so no setup is required.
        return origin;
    }

};

template<typename idType, typename originType = NullOrigin>
class IEvent {
public:
    typedef idType IdType;
    typedef originType OriginType;
    //static const bool OriginDefined = constexpr if (originType)
private:
    IdType eventId;
    OriginType _origin;
    mutable Context::Ptr _context;
public:
    IdType getEventId() const {
        return eventId;
    }

    void setEventId(IdType eventId) {
        IEvent::eventId = eventId;
    }

    OriginType &origin() {
        return _origin;
    }

    OriginType origin() const {
        return _origin;
    }

    static void setup(IEvent *event) {
//        LOGGER("IEVENT SETUP");
    };

    IEvent();


    Context::Ptr context() {
        return _context;
    }

    Context::Ptr context() const {
        return _context;
    }

};

template<typename idType, typename originType>
IEvent<idType, originType>::IEvent() {

    _context = Context::getActiveContext();

}

#endif //BASYCO_IEVENT_H
