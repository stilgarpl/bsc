//
// Created by stilgar on 26.11.18.
//

#ifndef BASYCO_LOGICSTATEEVENT_H
#define BASYCO_LOGICSTATEEVENT_H

#include "../IEvent.h"



enum class LogicStateMethod {
    entered,
    left,
    invalid,
};

template<typename Object, typename StateIdType>
class LogicStateEvent
        : public IEvent<StateIdType/*not so sure about this, ENTER and LEAVE should be separate events but now they aren't, but maybe that's not an issue, as it may be handled internally*/> {
public:
    typedef LogicStateMethod Method;
    typedef Object Type;
    using ExecutionPolicy = bsc::OrderedExecutionPolicy;
private:
    Object& object;
    Method method = Method::invalid;
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

    Object &getObject() {
        return object;
    }

    explicit LogicStateEvent(Object &object) : object(object) {}
};


#endif //BASYCO_LOGICSTATEEVENT_H
