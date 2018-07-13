//
// Created by stilgar on 06.11.17.
//

#ifndef BASYCO_JOURNALRESPONSEEVENT_H
#define BASYCO_JOURNALRESPONSEEVENT_H


#include <repo/journal/SimpleJournal.h>
#include <p2p/modules/nodeNetworkModule/protocol/logic/events/NetworkingEvent.h>

class JournalResponseEvent : public NetworkingEvent<int> {
private:

    JournalPtr journal;
    std::string repoId;
public:
    JournalResponseEvent(BasePacket::IdType requestId);

    const JournalPtr &getJournal() const;

    void setJournal(const JournalPtr journal);

    const std::string &getRepoId() const;

    void setRepoId(const std::string &repoId);
};


#endif //BASYCO_JOURNALRESPONSEEVENT_H
