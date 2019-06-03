//
// Created by stilgar on 04.07.18.
//

#ifndef BASYCO_AUTOSOURCE_H
#define BASYCO_AUTOSOURCE_H

#include <p2p/logic/ISource.h>
#include "EventQueueSource.h"

/**
 * source used by automated event generation from smart logic system
 */
class AutoSource : public ISource {

private:
    Uber<EventQueueSource> eventQueueSources;
public:
    explicit AutoSource(SourceManager &sourceManager);


    template<typename EventType, typename ... Args>
    void generateEvent(Args... args) {
//        LOGGER("generating event for type " + std::string(typeid(EventType).name()))
        auto &source = eventQueueSources.get<EventType, AutoSource>(std::ref(sourceManager));
        //will only start the thread if not started already.
        source.start();
        auto newEvent = source.newEvent(args...);
        source.queueEvent(newEvent);
    }

    void onStop() override;

    void run() override;

};


#endif //BASYCO_AUTOSOURCE_H
