//
// Created by stilgar on 24.03.19.
//

#ifndef BASYCO_TRIGGERSOURCE_H
#define BASYCO_TRIGGERSOURCE_H


#include <p2p/logic/ISource.h>
#include <p2p/logic/events/TriggerEvent.h>
#include "AutoSource.h"

class TriggerSource : public ISource {

private:
    AutoSource source;

public:

    explicit TriggerSource(SourceManager &sourceManager);

    template<typename TriggerIdType>
    void fireTrigger(TriggerIdType triggerId) {
        LOGGER("firing trigger " + triggerId)
        source.generateEvent<TriggerEvent<TriggerIdType>>(triggerId);
    }

    template<typename TriggerIdType, typename TriggerValueType>
    void fireTrigger(TriggerIdType triggerId, TriggerValueType value) {
        LOGGER("firing trigger " + triggerId + " with value " + value)
        source.generateEvent<TriggerEvent<TriggerIdType, TriggerValueType>>(triggerId, value);
    }

    void run() override;

    void onStop() override;

    void onStart() override;

};


#endif //BASYCO_TRIGGERSOURCE_H
