//
// Created by stilgar on 06.11.17.
//

#ifndef BASYCO_JOURNALRESPONSEEVENT_H
#define BASYCO_JOURNALRESPONSEEVENT_H


#include <repo/journal/Journal.h>
#include <p2p/network/logic/events/NetworkingEvent.h>

class JournalResponseEvent : public NetworkingEvent<int> {
private:

    Journal journal;
    std::string repoId;
public:
    const Journal &getJournal() const;

    void setJournal(const Journal &journal);

    const std::string &getRepoId() const;

    void setRepoId(const std::string &repoId);
};


#endif //BASYCO_JOURNALRESPONSEEVENT_H
