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
public:
    void work() override;

    explicit LogicStateSource(SourceManager &sourceManager);


    template<typename Object, typename StateIdType>
    void stateEntered(Object &object, const StateIdType &state) {
        LogicStateEvent<Object, StateIdType> logicEvent(object);
        logicEvent.setEventId(state);
        logicEvent.setMethod(LogicStateMethod::ENTERED);
        source.generateEvent<LogicStateEvent<Object, StateIdType>>(logicEvent);
    }

    template<typename Object, typename StateIdType>
    void stateLeft(Object &object, const StateIdType &state) {
        LogicStateEvent<Object, StateIdType> logicEvent(object);
        logicEvent.setEventId(state);
        logicEvent.setMethod(LogicStateMethod::LEFT);
        source.generateEvent<LogicStateEvent<Object, StateIdType>>(logicEvent);
    }
};


#endif //BASYCO_LOGICSTATESOURCE_H
