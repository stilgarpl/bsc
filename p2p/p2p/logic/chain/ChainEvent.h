#include <utility>

//
// Created by stilgar on 01.01.19.
//

#ifndef BASYCO_CHAINEVENT_H
#define BASYCO_CHAINEVENT_H

#include <string>
#include "ChainDefinitions.h"


template<typename ActualEventType>
class ChainEvent : public IEvent<ChainIdType> {
private:
    std::optional<ActualEventType> actualEvent;

public:

    std::optional<ActualEventType> getActualEvent() const {
        return actualEvent;
    }

    ChainEvent(ChainIdType stageId, ActualEventType actualEvent) : IEvent(stageId),
                                                                   actualEvent(actualEvent) {
        this->context()->setParentContext(actualEvent.context());


        this->context()->setDebug_id("chain event " + stageId);
    }

    ChainEvent() {
        actualEvent = std::nullopt;
        this->setEventValid(false);
        this->context()->setDebug_id("chain event");
    }
};


#endif //BASYCO_CHAINEVENT_H
