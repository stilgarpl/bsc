//
// Created by stilgar on 18.10.17.
//

#ifndef BASYCO_JOURNALEVENT_H
#define BASYCO_JOURNALEVENT_H


#include <logic/IEvent.h>
#include <repo/repository/Repository.h>
#include <p2p/modules/network/protocol/logic/events/NetworkingEvent.h>


enum class JournalEventId {
    JOURNAL_REQUEST
};

class JournalRequestEvent : public NetworkingEvent<JournalEventId> {

    //  Repository::
    std::string repoId;

public:

    JournalRequestEvent(BasePacket::IdType requestId);

    const std::string &getRepoId() const;

    void setRepoId(const std::string &repoId);


};


#endif //BASYCO_JOURNALEVENT_H
