//
// Created by stilgar on 18.10.17.
//

#ifndef BASYCO_JOURNALEVENT_H
#define BASYCO_JOURNALEVENT_H


#include <p2p/logic/IEvent.h>
#include <repo/repository/Repository.h>


enum class JournalEventId {
    JOURNAL_REQUEST
};

class JournalEvent : public IEvent<JournalEventId> {

    //  Repository::

};


#endif //BASYCO_JOURNALEVENT_H
