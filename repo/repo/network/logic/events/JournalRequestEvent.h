//
// Created by stilgar on 18.10.17.
//

#ifndef BASYCO_JOURNALEVENT_H
#define BASYCO_JOURNALEVENT_H


#include <p2p/logic/IEvent.h>
#include <repo/repository/Repository.h>
#include <p2p/network/logic/events/NetworkingEvent.h>


enum class JournalEventId {
    JOURNAL_REQUEST
};

class JournalRequestEvent : public NetworkingEvent<JournalEventId> {

    //  Repository::
    std::string repoId;

public:

    const std::string &getRepoId() const;

    void setRepoId(const std::string &repoId);


};


#endif //BASYCO_JOURNALEVENT_H
