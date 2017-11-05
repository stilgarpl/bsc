//
// Created by stilgar on 18.10.17.
//

#ifndef BASYCO_JOURNALSOURCE_H
#define BASYCO_JOURNALSOURCE_H


#include <p2p/logic/sources/EventQueueSource.h>
#include <repo/network/logic/events/JournalEvent.h>

class JournalSource : public EventQueueSource<JournalEvent> {

public:
    void journalRequested() {
        auto event = newEvent();
        queueEvent(event);
    };
};


#endif //BASYCO_JOURNALSOURCE_H
