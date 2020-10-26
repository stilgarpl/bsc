//
// Created by Krzysztof Tulidowicz on 23.08.17.
//

#ifndef BSC_IEVENT_H
#define BSC_IEVENT_H

#include <core/context/Context.h>
#include <core/signal/DefaultExecutionPolicy.h>
#include <core/log/Logger.h>


namespace bsc {
    template<typename idType>
    class IEvent {
    public:
        typedef idType IdType;
        using ExecutionPolicy = DefaultExecutionPolicy;
    private:
        IdType eventId;
        bool eventValid = true;
        Context::OwnPtr eventContext;
    public:
        IdType getEventId() const {
            return eventId;
        }

        void setEventId(IdType eventId) {
            IEvent::eventId = eventId;
        }

        [[nodiscard]] bool isEventValid() const { return eventValid; }

        void setEventValid(bool v) { eventValid = v; }

        IEvent();
        IEvent(const IEvent<idType>&);

        explicit IEvent(IdType e) : IEvent() { eventId = e; }

        [[nodiscard]] Context::Ptr context() { return eventContext; }

        [[nodiscard]] Context::Ptr context() const {
            return eventContext; }
    };

    template<typename idType>
    IEvent<idType>::IEvent() : eventContext(Context::makeContext(Context::getActiveContext())) {
        eventContext->setDebugId("event context");
    }
    template<typename idType>
    IEvent<idType>::IEvent(const IEvent<idType>& other) : eventId(other.eventId), eventContext(Context::makeContext(other.eventContext)) {}
}// namespace bsc
#endif //BSC_IEVENT_H
