//
// Created by stilgar on 26.11.18.
//

#ifndef BASYCO_LOGICSTATESOURCE_H
#define BASYCO_LOGICSTATESOURCE_H


#include "../ISource.h"
#include "../events/LogicStateEvent.h"
#include "AutoSource.h"
#include <core/signal/OrderedExecutor.h>
#include <core/signal/OrderedExecutionPolicy.h>

BSC_MAP_POLICY_TO_EXECUTOR(bsc::OrderedExecutionPolicy, bsc::OrderedExecutor)

class LogicStateSource : public ISource {

private:
    AutoSource source;
private:
    template<typename EventType>
    void selectExecutionPolicy() {
        //@todo think of better way of selecting it, so it isn't run on every event.
        //  probably the best way would be to have init metod in autosource that would run once on creation of every container.
        //  so Uber should have that one init method.
        //@todo remove this function
//        sourceManager.setExecutionPolicy<EventType>(std::make_shared<OrderedExecutionPolicy>());
    }
public:

    explicit LogicStateSource(SourceManager &sourceManager);


    template<typename Object, typename StateIdType>
    void stateEntered(Object &object, const StateIdType &state) {
        LogicStateEvent<Object, StateIdType> logicEvent(object);
        logicEvent.setEventId(state);
        logicEvent.setMethod(LogicStateMethod::entered);
        source.generateEvent<LogicStateEvent<Object, StateIdType>>(logicEvent);
    }

    template<typename Object, typename StateIdType>
    void stateLeft(Object &object, const StateIdType &state) {
        LogicStateEvent<Object, StateIdType> logicEvent(object);
        logicEvent.setEventId(state);
        logicEvent.setMethod(LogicStateMethod::left);
        source.generateEvent<LogicStateEvent<Object, StateIdType>>(logicEvent);
    }

    ~LogicStateSource() override;

    void run() override;

    void onStop() override;

    void onStart() override;
};


#endif //BASYCO_LOGICSTATESOURCE_H
