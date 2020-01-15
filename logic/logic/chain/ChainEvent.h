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


namespace bsc {
    template<typename ActualEventType>
    class ChainEvent : public bsc::IEvent<ChainIdType> {
    private:
        ChainIdType baseChainId;
        std::optional<ChainLockIdType> chainLockId;
        std::optional<ActualEventType> actualEvent;
        InstanceType instance;
    public:

        std::optional<ActualEventType> getActualEvent() const {
            return actualEvent;
        }

        ChainEvent(ChainIdType base, const ChainIdType& stageId, InstanceType instance, ActualEventType actualEvent,
                   std::optional<ChainLockIdType> chainLockId) :
                IEvent(stageId), baseChainId(std::move(base)), chainLockId(std::move(chainLockId)),
                actualEvent(actualEvent), instance(instance) {
            //        this->context()->setParentContext(actualEvent.context());
            this->context()->setDebugId("chain event " + stageId);
        }

        ChainEvent() {
            actualEvent = std::nullopt;
            instance = 0;
            this->setEventValid(false);
            this->context()->setDebugId("chain event");
        }

        InstanceType getInstance() const {
            return instance;
        }

        const ChainIdType& getBaseChainId() const {
            return baseChainId;
        }

        const std::optional<ChainLockIdType>& getChainLockId() const {
            return chainLockId;
        }

        void setChainLockId(const std::optional<ChainLockIdType>& chainLockId) {
            ChainEvent::chainLockId = chainLockId;
        }
    };
}


#endif //BASYCO_CHAINEVENT_H
