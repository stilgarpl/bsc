//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_IEVENT_H
#define BASYCO_IEVENT_H

#include <cstdlib>

typedef std::nullptr_t NullOrigin;

template<typename OriginType>
struct setupOrigin {
    static OriginType &setup(OriginType &origin) {
        ///@todo log some error of unimplemented origin setup

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

};

#endif //BASYCO_IEVENT_H
