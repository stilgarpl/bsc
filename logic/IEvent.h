//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_IEVENT_H
#define BASYCO_IEVENT_H

template<typename idType>
class IEvent {
public:
    typedef idType IdType;
private:
    IdType eventId;
public:
    IdType getEventId() const {
        return eventId;
    }

    void setEventId(IdType eventId) {
        IEvent::eventId = eventId;
    }
};

#endif //BASYCO_IEVENT_H
