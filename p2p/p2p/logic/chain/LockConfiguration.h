//
// Created by stilgar on 4/18/19.
//

#ifndef BASYCO_LOCKCONFIGURATION_H
#define BASYCO_LOCKCONFIGURATION_H


#include "ChainDefinitions.h"
#include <functional>

template<typename EventType>
class LockConfiguration {
    std::function<ChainLockIdType(const EventType &)> lockEvaluator;

public:

    template<typename ... Evaluators>
    explicit LockConfiguration(Evaluators ... evaluators) {
        lockEvaluator = [=](const EventType &event) {
            return (evaluators(event)+...);
        };
    }

    //@todo this may be pure virtual and this whole implementation may be just combinedLockConfigurator... or not. it may be fine as it is
    ChainLockIdType getLockId(const EventType &event) const {
        return lockEvaluator(event);
    }

};


#endif //BASYCO_LOCKCONFIGURATION_H
