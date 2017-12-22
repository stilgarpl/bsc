//
// Created by stilgar on 18.10.17.
//

#ifndef BASYCO_JOURNALSOURCE_H
#define BASYCO_JOURNALSOURCE_H


#include <p2p/logic/sources/EventQueueSource.h>
#include <repo/journal/network/logic/events/JournalRequestEvent.h>
#include <repo/journal/network/logic/events/JournalResponseEvent.h>
#include <repo/journal/network/packet/JournalGroup.h>

class JournalSource : public ISource {
private:
    EventQueueSource<JournalRequestEvent, JournalSource> requestSource;
    EventQueueSource<JournalResponseEvent, JournalSource> responseSource;
public:
    void journalRequested(std::string repoId, JournalGroup::Request::IdType requestId) {
        auto event = requestSource.newEvent();
        event->setRepoId(repoId);
        event->setRequestId(requestId);
        requestSource.queueEvent(event);
    };

    void journalReceived(std::string repoId, const JournalPtr journal) {
        auto event = responseSource.newEvent();
        event->setRepoId(repoId);
        event->setJournal(journal);
        responseSource.queueEvent(event);
    }

    void work() override;

    void registerProviders(SourceManager *manager) override;
};


#endif //BASYCO_JOURNALSOURCE_H
