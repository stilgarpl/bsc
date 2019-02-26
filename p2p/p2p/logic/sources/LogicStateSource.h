//
// Created by stilgar on 26.11.18.
//

#ifndef BASYCO_LOGICSTATESOURCE_H
#define BASYCO_LOGICSTATESOURCE_H


#include <p2p/logic/ISource.h>
#include <p2p/logic/events/LogicStateEvent.h>
#include "AutoSource.h"

class LogicStateSource : public ISource {

private:
    AutoSource source;
private:
    template<typename EventType>
    void selectExecutionPolicy() {
        //@todo think of better way of selecting it, so it isn't run on every event.
        //  probably the best way would be to have init metod in autosource that would run once on creation of every container.
        //  so Uber should have that one init method.  
        sourceManager.setExecutionPolicy<EventType>(std::make_shared<OrderedExecutionPolicy>());
    }
public:
    void work() override;

    explicit LogicStateSource(SourceManager &sourceManager);


    template<typename Object, typename StateIdType>
    void stateEntered(Object &object, const StateIdType &state) {
        selectExecutionPolicy<LogicStateEvent<Object, StateIdType>>();
        LogicStateEvent<Object, StateIdType> logicEvent(object);
        logicEvent.setEventId(state);
        logicEvent.setMethod(LogicStateMethod::ENTERED);
        source.generateEvent<LogicStateEvent<Object, StateIdType>>(logicEvent);
    }

    template<typename Object, typename StateIdType>
    void stateLeft(Object &object, const StateIdType &state) {
        selectExecutionPolicy<LogicStateEvent<Object, StateIdType>>();
        LogicStateEvent<Object, StateIdType> logicEvent(object);
        logicEvent.setEventId(state);
        logicEvent.setMethod(LogicStateMethod::LEFT);
        source.generateEvent<LogicStateEvent<Object, StateIdType>>(logicEvent);
    }

    virtual ~LogicStateSource();
};


#endif //BASYCO_LOGICSTATESOURCE_H
