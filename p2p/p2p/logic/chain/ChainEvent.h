#include <utility>

#include <utility>

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
    ChainIdType baseChainId;
    ChainLockIdType chainLockId;
    std::optional<ActualEventType> actualEvent;
    InstanceType instance;
public:

    std::optional<ActualEventType> getActualEvent() const {
        return actualEvent;
    }

    ChainEvent(ChainIdType base, const ChainIdType& stageId, InstanceType instance, ActualEventType actualEvent,
               ChainLockIdType chainLockId) :
            IEvent(stageId), baseChainId(std::move(base)),chainLockId(std::move(chainLockId)) , actualEvent(actualEvent), instance(instance)
            {
//        this->context()->setParentContext(actualEvent.context());
        this->context()->setDebug_id("chain event " + stageId);
    }

    ChainEvent() {
        actualEvent = std::nullopt;
        instance = 0;
        this->setEventValid(false);
        this->context()->setDebug_id("chain event");
    }

    InstanceType getInstance() const {
        return instance;
    }

    const ChainIdType &getBaseChainId() const {
        return baseChainId;
    }

    const ChainLockIdType &getChainLockId() const {
        return chainLockId;
    }
};


#endif //BASYCO_CHAINEVENT_H
