//
// Created by stilgar on 06.11.17.
//

#ifndef BASYCO_JOURNALACTIONS_H
#define BASYCO_JOURNALACTIONS_H


#include <repo/journal/network/logic/events/JournalRequestEvent.h>
#include <repo/journal/network/logic/events/JournalResponseEvent.h>
#include <repo/journal/network/packet/JournalGroup.h>
#include <p2p/node/context/NodeContext.h>
#include <repo/node/RepoModule.h>

class JournalActions {

public:

    static void journalRequested(const JournalRequestEvent &event);

    static void journalReceived(const JournalResponseEvent &event) {

        //verify journal
        //merge jorunal
        //possibly replay journal

    }

};


#endif //BASYCO_JOURNALACTIONS_H
