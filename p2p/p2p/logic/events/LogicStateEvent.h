//
// Created by stilgar on 26.11.18.
//

#ifndef BASYCO_LOGICSTATEEVENT_H
#define BASYCO_LOGICSTATEEVENT_H

#include <p2p/logic/IEvent.h>


enum class LogicStateMethod {
    ENTERED,
    LEFT,
    INVALID,
};

template<typename Object, typename StateIdType>
class LogicStateEvent
        : public IEvent<StateIdType/*not so sure about this, ENTER and LEAVE should be separate events but now they aren't, but maybe that's not an issue, as it may be handled internally*/> {
public:
    typedef LogicStateMethod Method;
    typedef Object Type;
private:
    Object &object;
    Method method = Method::INVALID;
public :
    StateIdType getState() {
        return this->getEventId();
    }

    Method getMethod() const {
        return method;
    }

    void setMethod(Method method) {
        LogicStateEvent::method = method;
    }

    explicit LogicStateEvent(Object &object) : object(object) {}
};


#endif //BASYCO_LOGICSTATEEVENT_H
