//
// Created by Krzysztof Tulidowicz on 26.11.18.
//

#ifndef BSC_LOGICSTATEEVENT_H
#define BSC_LOGICSTATEEVENT_H

#include <core/signal/OrderedExecutionPolicy.h>
#include <logic/IEvent.h>

namespace bsc {
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
        using ExecutionPolicy = OrderedExecutionPolicy;

    private:
        Object& object;
        Method method = Method::invalid;

    public:
        StateIdType getState() { return this->getEventId(); }

        Method getMethod() const {
            return method;
        }

        void setMethod(Method method) {
            LogicStateEvent::method = method;
        }

        Object& getObject() {
            return object;
        }

        explicit LogicStateEvent(Object& object) : object(object) {}
    };
}


#endif //BSC_LOGICSTATEEVENT_H
