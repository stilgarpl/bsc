//
// Created by stilgar on 06.11.17.
//

#ifndef BASYCO_JOURNALRESPONSEEVENT_H
#define BASYCO_JOURNALRESPONSEEVENT_H


#include <repo/journal/SimpleJournal.h>
#include <p2p/modules/network/protocol/logic/events/NetworkingEvent.h>
#include <repo/repository/IRepository.h>

class JournalResponseEvent : public NetworkingEvent<int> {
private:

    JournalPtr journal;
    IRepository::RepoIdType repoId;
public:

    const JournalPtr &getJournal() const;

    void setJournal(const JournalPtr journal);

    const IRepository::RepoIdType &getRepoId() const;

    void setRepoId(const IRepository::RepoIdType &repoId);
};


#endif //BASYCO_JOURNALRESPONSEEVENT_H
