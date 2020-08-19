//
// Created by Krzysztof Tulidowicz on 04.07.18.
//

#ifndef BSC_AUTOSOURCE_H
#define BSC_AUTOSOURCE_H

#include "../ISource.h"
#include "EventQueueSource.h"

#include <core/uber/Uber.h>


namespace bsc {
    /**
         * source used by automated event generation from smart logic system
         */
    class AutoSource : public ISource {

    private:
        Uber<EventQueueSource> eventQueueSources;
        std::mutex sourcesLock;
        std::atomic_bool active = true;

    public:
        explicit AutoSource(SourceManager& sourceManager);


        template<typename EventType, typename ... Args>
        void generateEvent(Args... args) {
            std::unique_lock g(sourcesLock);
            if (active) {
                //        LOGGER("generating event for type " + std::string(typeid(EventType).name()))
                auto& source = eventQueueSources.get<EventType, AutoSource>(std::ref(sourceManager));
                //will only start the thread if not started already.
                source.start();
                auto newEvent = source.newEvent(args...);
                source.queueEvent(newEvent);
            }
        }

        void onStop() override;

        void run() override;

    };
}


#endif //BSC_AUTOSOURCE_H
