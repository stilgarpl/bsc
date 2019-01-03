#include <utility>

//
// Created by stilgar on 01.01.19.
//

#ifndef BASYCO_CHAINEVENT_H
#define BASYCO_CHAINEVENT_H

#include <string>

using ChainIdType = std::string;

template<typename ActualEventType>
class ChainEvent : public IEvent<typename ActualEventType::IdType> {
private:
    ChainIdType stageId;
    std::optional<ActualEventType> actualEvent;

public:
    const ChainIdType &getStageId() const {
        return stageId;
    }

    std::optional<ActualEventType> getActualEvent() const {
        return actualEvent;
    }

    ChainEvent(ChainIdType stageId, ActualEventType actualEvent) : stageId(std::move(stageId)),
                                                                   actualEvent(actualEvent) {}

    ChainEvent() { actualEvent = std::nullopt; }
};


#endif //BASYCO_CHAINEVENT_H
