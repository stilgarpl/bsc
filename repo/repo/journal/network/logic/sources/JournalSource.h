//
// Created by stilgar on 18.10.17.
//

#ifndef BASYCO_JOURNALSOURCE_H
#define BASYCO_JOURNALSOURCE_H


#include <logic/sources/EventQueueSource.h>
#include <repo/journal/network/logic/events/JournalRequestEvent.h>
#include <repo/journal/network/logic/events/JournalResponseEvent.h>
#include <repo/journal/network/packet/JournalGroup.h>

class JournalSource : public ISource {
private:
    EventQueueSource<JournalRequestEvent, JournalSource> requestSource;
    EventQueueSource<JournalResponseEvent, JournalSource> responseSource;
public:
    explicit JournalSource(SourceManager &sourceManager);

    void journalRequested(const std::string &repoId, JournalGroup::Request::IdType requestId);

    void journalReceived(const std::string &repoId, const JournalPtr &journal);

    void run() override;

    void onStop() override;

    void onStart() override;

};


#endif //BASYCO_JOURNALSOURCE_H
