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
    void work() override;

    explicit TriggerSource(SourceManager &sourceManager);

    template<typename TriggerIdType>
    void fireTrigger(TriggerIdType triggerId) {
        LOGGER("firing trigger " + triggerId)
        source.generateEvent<TriggerEvent<TriggerIdType>>(triggerId);
    }

};


#endif //BASYCO_TRIGGERSOURCE_H
