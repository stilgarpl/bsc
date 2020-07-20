//
// Created by Krzysztof Tulidowicz on 24.03.19.
//

#ifndef BSC_TRIGGERSOURCE_H
#define BSC_TRIGGERSOURCE_H


#include <logic/events/TriggerEvent.h>
#include <logic/ISource.h>
#include <logic/sources/AutoSource.h>

namespace bsc {
    class TriggerSource : public ISource {

    private:
        AutoSource source;

    public:

        explicit TriggerSource(SourceManager& sourceManager);

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
}


#endif //BSC_TRIGGERSOURCE_H
