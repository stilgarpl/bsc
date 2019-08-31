//
// Created by stilgar on 18.10.17.
//

#ifndef BASYCO_JOURNALEVENT_H
#define BASYCO_JOURNALEVENT_H


#include <logic/IEvent.h>
#include <repo/repository/IRepository.h>
#include <p2p/modules/network/protocol/logic/events/NetworkingEvent.h>


enum class JournalEventId {
    JOURNAL_REQUEST
};

class JournalRequestEvent : public NetworkingEvent<JournalEventId> {

    IRepository::RepoIdType repoId;

public:

    const IRepository::RepoIdType &getRepoId() const;

    void setRepoId(const IRepository::RepoIdType &repoId);


};


#endif //BASYCO_JOURNALEVENT_H
