//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_IEVENT_H
#define BASYCO_IEVENT_H

#include <iostream>

template<typename idType, typename originType = std::nullptr_t>
class IEvent {
public:
    typedef idType IdType;
    typedef originType OriginType;
private:
    IdType eventId;
    OriginType _origin;
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

};

#endif //BASYCO_IEVENT_H
